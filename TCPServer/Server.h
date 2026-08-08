#pragma once

#include <map>
#include <string>
#include <ctime>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

struct Session
{
	SOCKET SocketId;
	std::string IdToken;
	std::time_t ConnectedAt;
	std::time_t TokenExpiresIn;
};

class Server
{
public:
	Server();
	~Server();

private:
	std::map<std::string, Session> SessionList;

	WSAData wsaData;
	SOCKET ListenSock;
	const u_short ServerPort = 34567;

	fd_set ReadSet;
	fd_set CopySet;
	const timeval TimeOut = { 0, 500000 };

public:
	void ServerStart();

private:
	int InitListenSock();
	void FindChangedInReadSet();
	void AcceptClient();
	void DisconnectClient(SOCKET& ClientSock);
	bool RecvFromClient(SOCKET& ClientSock, char* RecvBuffer);
	void SendToClient(SOCKET& ClientSock, const char* Buffer);
};

