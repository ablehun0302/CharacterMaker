#pragma once
#define NOMINMAX

#include <map>
#include <string>
#include <ctime>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

struct Session
{
	SOCKET SocketId;
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
	void Accept();
	void Disconnect(SOCKET& ClientSock);
	bool RecvAll(SOCKET& ClientSock, char* OutBuffer);
	
	void SendAll(SOCKET& ClientSock, const uint8_t* Buffer, uint32_t DataSize);
	void ProcessPacket(SOCKET& ClientSock, const char* InBuffer);
};

