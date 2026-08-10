// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClientSubsystem.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "ThirdParty/UserPacket_generated.h"

void UTCPClientSubsystem::Deinitialize()
{
	Disconnect();
}

void UTCPClientSubsystem::ConnectServer(const FString& Host, const int32 Port)
{
	ISocketSubsystem* SocketSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	FAddressInfoResult AddressInfo = SocketSystem->GetAddressInfo(*Host, nullptr, EAddressInfoFlags::Default, NAME_None);
	TSharedRef<FInternetAddr> ServerAddr = AddressInfo.Results[0].Address;
	ServerAddr->SetPort(Port);

	ServerSocket = SocketSystem->CreateSocket(NAME_Stream, TEXT("TCPClient"), ServerAddr->GetProtocolType());

	if (!ServerSocket->Connect(*ServerAddr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Connect Error"));
		return;
	}

	ServerSocket->SetNonBlocking(false);

	UE_LOG(LogTemp, Display, TEXT("TCP Connect!!"));
}

void UTCPClientSubsystem::Disconnect()
{
	if (ServerSocket)
	{
		ISocketSubsystem* SocketSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		
		ServerSocket->Close();
		SocketSystem->DestroySocket(ServerSocket);
		ServerSocket = nullptr;
	}
	
}

bool UTCPClientSubsystem::IsConnected()
{
	return ServerSocket != nullptr && ServerSocket->GetConnectionState() == SCS_Connected;
}

void UTCPClientSubsystem::SendLogin(const FString& InIdToken, const FString& InExpiresIn)
{
	flatbuffers::FlatBufferBuilder Builder;

	FTCHARToUTF8 IdToken(*InIdToken);
	int64_t ExpiresIn = FCString::Atoi64(*InExpiresIn);

	auto C2S_LoginData = UserPacket::CreateC2S_LoginDirect
	(
		Builder, IdToken.Get(), ExpiresIn
	);
	auto PacketData = UserPacket::CreatePacketData
	(
		Builder, UserPacket::PacketType_C2S_Login, C2S_LoginData.Union()
	);

	Builder.Finish(PacketData);

	UE_LOG(LogTemp, Display, TEXT("Body Length: %d"), Builder.GetSize());

	SendAll(Builder.GetBufferPointer(), Builder.GetSize());
}

bool UTCPClientSubsystem::SendAll(const uint8* InBody, uint32 InBodyLength)
{
	TArray<uint8> Packet;

	// 헤더 = 길이 2바이트 + 데이터
	Packet.Reserve(2 + InBodyLength);
	// 길이
	Packet.Add((uint8)((InBodyLength >> 8) & 0xFF));
	Packet.Add((uint8) (InBodyLength & 0xFF));
	// 데이터
	Packet.Append(InBody, InBodyLength);

	int TotalSentBytes = 0;
	while (TotalSentBytes < Packet.Num())
	{
		int SentBytes = 0;
		if (!ServerSocket->Send(Packet.GetData() + TotalSentBytes, Packet.Num() - TotalSentBytes, SentBytes) || SentBytes < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SendAll Error"));
			return false;
		}

		TotalSentBytes += SentBytes;
	}

	UE_LOG(LogTemp, Display, TEXT("SendAll Complete: %d byte"), TotalSentBytes);
	return true;
}
