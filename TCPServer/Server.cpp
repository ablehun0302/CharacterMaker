#include "Server.h"
#include <iostream>
#include <WinSock2.h>

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
				AcceptClient();
			}
			else // 클라이언트 소켓 = 신호 받고 답장
			{
				char RecvBuffer[256] = { 0, };
				bool IsSuccessRecv = RecvFromClient(ReadSet.fd_array[i], RecvBuffer);
				if (IsSuccessRecv)
				{
					SendToClient(ReadSet.fd_array[i], RecvBuffer);
				}
			}
		}
	}
}

void Server::AcceptClient()
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

void Server::DisconnectClient(SOCKET& ClientSock)
{
	std::cout << "disconnect: " << ClientSock << std::endl;
	SOCKET ClosedSocket = ClientSock;
	FD_CLR(ClientSock, &ReadSet);
	closesocket(ClosedSocket);
}

bool Server::RecvFromClient(SOCKET& ClientSock, char* RecvBuffer)
{
	int Result = recv(ClientSock, RecvBuffer, 256, MSG_WAITALL);
	if (Result <= 0)
	{
		DisconnectClient(ClientSock);
		return false;
	}
	std::cout << "Client: " << RecvBuffer << std::endl;
	return true;
}

void Server::SendToClient(SOCKET& ClientSock, const char* Buffer)
{
	int Result = send(ClientSock, Buffer, 256, 0);
	if (Result <= 0)
	{
		DisconnectClient(ClientSock);
		return;
	}
	std::cout << "Server: " << Buffer << std::endl;
}
