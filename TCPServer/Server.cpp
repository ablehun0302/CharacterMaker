#include "Server.h"
#include <iostream>
#include <vector>
#include "flatbuffers/flatbuffers.h"
#include "UserPacket_generated.h"

#pragma comment(lib, "ws2_32")

Server::Server()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

Server::~Server()
{
	closesocket(ListenSock);
	WSACleanup();
}

void Server::ServerStart()
{
	InitListenSock();

	FD_ZERO(&ReadSet);
	FD_SET(ListenSock, &ReadSet);

	int ChangedSocketCount = 0;
	while (true)
	{
		CopySet = ReadSet;
		ChangedSocketCount = select(0, &CopySet, 0, 0, &TimeOut);

		if (ChangedSocketCount == 0)
		{
			//서버 동작
			continue;
		}
		else if (ChangedSocketCount == SOCKET_ERROR)
		{
			std::cout << "Select Error" << std::endl;
			continue;
		}
		FindChangedInReadSet();
	}
}

int Server::InitListenSock()
{
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenSockAddr.sin_port = htons(ServerPort);

	if (bind(ListenSock, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr)) != 0)
	{
		std::cout << "bind error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	if (listen(ListenSock, SOMAXCONN) != 0)
	{
		std::cout << "listen error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	return 0;
}

void Server::FindChangedInReadSet()
{
	for (int i = 0; i < (int)ReadSet.fd_count; i++)
	{
		// 바뀐게 있나?
		if (FD_ISSET(ReadSet.fd_array[i], &CopySet))
		{
			if (ReadSet.fd_array[i] == ListenSock) // 리슨 소켓 = 클라이언트 받기
			{
				Accept();
			}
			else // 클라이언트 소켓 = 신호 받고 답장
			{
				char RecvBuffer[65535] = { 0, };
				bool IsSuccessRecv = RecvAll(ReadSet.fd_array[i], RecvBuffer);
				if (IsSuccessRecv)
				{
					ProcessPacket(ReadSet.fd_array[i], RecvBuffer);
				}
			}
		}
	}
}

void Server::Accept()
{
	SOCKET ClientSock = NULL;
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int LengthClientSockAddr = sizeof(ClientSockAddr);

	ClientSock = accept(ListenSock, (SOCKADDR*)&ClientSockAddr, &LengthClientSockAddr);
	if (ClientSock != INVALID_SOCKET)
	{
		FD_SET(ClientSock, &ReadSet);
		std::cout << "Client Connect: " << ClientSock << std::endl;
	}
	else
	{
		std::cout << "Connect Fail: " << ClientSock << std::endl;
	}
}

void Server::Disconnect(SOCKET& ClientSock)
{
	std::cout << "disconnect: " << ClientSock << std::endl;
	SOCKET ClosedSocket = ClientSock;
	FD_CLR(ClientSock, &ReadSet);
	closesocket(ClosedSocket);
}

bool Server::RecvAll(SOCKET& ClientSock, char* OutBuffer)
{
	u_short BodyLength = 0;

	int Result = recv(ClientSock, (char*)&BodyLength, sizeof(BodyLength), MSG_WAITALL);
	if (Result <= 0)
	{
		std::cout << "BodyLength Recv Fail: " << Result << std::endl;
		Disconnect(ClientSock);
		return false;
	}

	//std::cout << "BodyLength: " << BodyLength << std::endl;
	//std::cout << "BodyLength: " << ntohs(BodyLength) << std::endl;

	Result = recv(ClientSock, OutBuffer, ntohs(BodyLength), MSG_WAITALL);
	if (Result <= 0)
	{
		std::cout << "Body Recv Fail: " << Result << std::endl;
		Disconnect(ClientSock);
		return false;
	}
	
	return true;
}

void Server::SendAll(SOCKET& ClientSock, const uint8_t* Buffer, uint32_t DataSize)
{
	// 헤더 + 데이터 보내기
	std::vector<char> Packet;
	size_t PacketSize = 2 + (size_t)DataSize;
	Packet.resize(PacketSize);
	// 헤더
	u_short NetDataSize = htons((u_short)DataSize);
	memcpy(Packet.data(), &NetDataSize, sizeof(NetDataSize));
	// 데이터
	memcpy(Packet.data() + 2, Buffer, (size_t)DataSize);

	int TotalSentBytes = 0;
	do
	{
		int SentBytes = send(ClientSock, Packet.data() + TotalSentBytes, PacketSize - TotalSentBytes, 0);
		TotalSentBytes += SentBytes;

		if (SentBytes <= 0)
		{
			std::cout << "Send Fail: " << SentBytes << std::endl;
		}
	} while (TotalSentBytes < PacketSize);
}

void Server::ProcessPacket(SOCKET& ClientSock, const char* InBuffer)
{
	auto PacketData = UserPacket::GetPacketData(InBuffer);

	switch (PacketData->data_type())
	{
		case UserPacket::PacketType_C2S_Login:
		{
			// Login Data 가져오기
			auto C2SLoginData = PacketData->data_as_C2S_Login();
			std::string UID = C2SLoginData->uid()->c_str();
			std::cout << "   UID: " << UID << std::endl;

			// 세션 맵에 UID 키가 없다면 넣고 아님말고
			Session NewSession = { ClientSock };
			auto InsertResult = SessionList.insert({ UID, NewSession });

			// 결과에 맞는 데이터 보내기 Server -> Client
			if (InsertResult.second)
			{
				std::cout << "New session" << std::endl;
				flatbuffers::FlatBufferBuilder Builder;

				auto S2C_LoginData = UserPacket::CreateS2C_Login
				(
					Builder, true
				);
				auto PacketData = UserPacket::CreatePacketData
				(
					Builder, UserPacket::PacketType_S2C_Login, S2C_LoginData.Union()
				);

				Builder.Finish(PacketData);
				SendAll(ClientSock, Builder.GetBufferPointer(), Builder.GetSize());
			}
			else
			{
				std::cout << "Duplicated session" << std::endl;
			}

			break;
		}
		default:
			break;
	}
}
