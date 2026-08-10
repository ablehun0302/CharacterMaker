#include "Server.h"
#include <iostream>
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
					ProcessPacket(RecvBuffer);
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

void Server::SendAll(SOCKET& ClientSock, const char* Buffer)
{
	int Result = send(ClientSock, Buffer, 256, 0);
	if (Result <= 0)
	{
		Disconnect(ClientSock);
		return;
	}
	std::cout << "Server: " << Buffer << std::endl;
}

void Server::ProcessPacket(const char* InBuffer)
{
	auto PacketData = UserPacket::GetPacketData(InBuffer);

	switch (PacketData->data_type())
	{
		case UserPacket::PacketType_C2S_Login:
		{
			auto C2SLoginData = PacketData->data_as_C2S_Login();

			std::cout << "   IdToken: " << C2SLoginData->id_token()->c_str() << std::endl;
			std::cout << "   Token Expires In: " << C2SLoginData->token_expires_in() << std::endl;

			break;
		}
		default:
			break;
	}
}
