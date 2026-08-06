#include <iostream>
#include <WinSock2.h>


#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenSockAddr.sin_port = htons(34567);

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

	fd_set ReadSet;
	fd_set CopySet;
	const timeval TimeOut = {0, 500000};

	FD_ZERO(&ReadSet);
	FD_SET(ListenSock, &ReadSet);

	while (true)
	{
		CopySet = ReadSet;
		int ChangedSocketCount = select(0, &CopySet, 0, 0, &TimeOut);
		if (ChangedSocketCount == 0)
		{
			//서버 동작
			continue;
		}
		else if (ChangedSocketCount == SOCKET_ERROR)
		{
			std::cout << "Select Error" << std::endl;
			return -1;
		}
		
		for (int i = 0; i < (int)ReadSet.fd_count; i++)
		{
			// 바뀐게 있나?
			if (FD_ISSET(ReadSet.fd_array[i], &CopySet))
			{
				if (ReadSet.fd_array[i] == ListenSock) // 리슨 소켓 = 클라이언트 받기
				{
					SOCKET ClientSock = NULL;
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int LengthClientSockAddr = sizeof(ClientSockAddr);

					ClientSock = accept(ListenSock, (SOCKADDR*)&ClientSockAddr, &LengthClientSockAddr);
					FD_SET(ClientSock, &ReadSet);
				}
				else // 클라이언트 소켓 = 신호 받고 답장
				{
					char RecvBuffer[256] = { 0, };
					int Result = recv(ReadSet.fd_array[i], RecvBuffer, sizeof(RecvBuffer), MSG_WAITALL);
					if (Result <= 0)
					{
						std::cout << "disconnect" << std::endl;
						SOCKET ClosedSocket = ReadSet.fd_array[i];
						FD_CLR(ReadSet.fd_array[i], &ReadSet);
						closesocket(ClosedSocket);
						continue;
					}
					std::cout << "Client: " << RecvBuffer << std::endl;

					Result = send(ReadSet.fd_array[i], RecvBuffer, sizeof(RecvBuffer), 0);
					if (Result <= 0)
					{
						std::cout << "disconnect" << std::endl;
						SOCKET ClosedSocket = ReadSet.fd_array[i];
						FD_CLR(ReadSet.fd_array[i], &ReadSet);
						closesocket(ClosedSocket);
						continue;
					}
					std::cout << "Server: " << RecvBuffer << std::endl;

				}
			}
		}
	}


	closesocket(ListenSock);
	WSACleanup();
	return 0;
}