#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>

#pragma comment(lib, "ws2_32")

bool IsSendThreadRunning = true;
bool IsRecvThreadRunning = true;

unsigned WINAPI SendThread(void* Param)
{
	SOCKET ServerSock = *(SOCKET*)Param;

	while (IsSendThreadRunning)
	{
		char SendBuffer[256] = { 0, };
		std::cin.getline(SendBuffer, sizeof(SendBuffer));
		int Result = send(ServerSock, SendBuffer, sizeof(SendBuffer), 0);
		if (Result == 0)
		{
			std::cout << "disconnect" << std::endl;
			return 0;
		}
		else if (Result == SOCKET_ERROR)
		{
			std::cout << "send error: " << WSAGetLastError() << std::endl;
			return -1;
		}
		std::cout << "Client: " << SendBuffer << std::endl;
	}

	return 0;
}

unsigned WINAPI RecvThread(void* Param)
{
	SOCKET ServerSock = *(SOCKET*)Param;

	while (IsRecvThreadRunning)
	{
		char RecvBuffer[256] = { 0, };
		int Result = recv(ServerSock, RecvBuffer, sizeof(RecvBuffer), MSG_WAITALL);
		if (Result == 0)
		{
			std::cout << "disconnect" << std::endl;
			return 0;
		}
		else if (Result == SOCKET_ERROR)
		{
			std::cout << "recv error: " << WSAGetLastError() << std::endl;
			return -1;
		}
		std::cout << "Server: " << RecvBuffer << std::endl;
	}

	return 0;
}

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &ServerSockAddr.sin_addr);
	ServerSockAddr.sin_port = htons(34567);

	if (connect(ServerSock, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr)) != 0)
	{
		std::cout << "connect error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	HANDLE ThreadHandles[2] = { 0, };

	ThreadHandles[0] = (HANDLE)_beginthreadex(NULL, 0, SendThread, &ServerSock, 0, nullptr);
	ThreadHandles[1] = (HANDLE)_beginthreadex(NULL, 0, RecvThread, &ServerSock, 0, nullptr);
	
	WaitForMultipleObjects(2, ThreadHandles, FALSE, INFINITE);

	CloseHandle(ThreadHandles[0]);
	CloseHandle(ThreadHandles[1]);

	IsSendThreadRunning = false;
	IsRecvThreadRunning = false;

	closesocket(ServerSock);
	WSACleanup();
	return 0;
}