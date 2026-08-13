// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPRecvWorker.h"
#include "Sockets.h"
#include "TimerManager.h"

#include "ThirdParty/UserPacket_generated.h"

FTCPRecvWorker::FTCPRecvWorker(FSocket* InServerSocket, TQueue<TArray<uint8>>& InRecvQueue) : ServerSocket(InServerSocket), RecvQueue(InRecvQueue)
{
}

FTCPRecvWorker::~FTCPRecvWorker()
{
}

uint32 FTCPRecvWorker::Run()
{
	while (IsRunning && ServerSocket)
	{
		uint32 Pending = 0;
		if (!ServerSocket->HasPendingData(Pending))
		{
			continue;
		}

		// 데이터 사이즈 받기
		uint16 BodyLength = 0;
		int32 TotalReadBytes = 0;
		bool IsHeaderReady = false;
		while (TotalReadBytes < (int32)sizeof(BodyLength))
		{
			int32 ReadBytes = 0;
			IsHeaderReady = ServerSocket->Recv((uint8*)&BodyLength + TotalReadBytes, sizeof(BodyLength) - TotalReadBytes, ReadBytes);
			if (!IsHeaderReady || ReadBytes == 0)
			{
				IsHeaderReady = false;
				break;
			}

			TotalReadBytes += ReadBytes;
		}

		// 헤더 준비 됐나?
		if (!IsHeaderReady)
		{
			UE_LOG(LogTemp, Warning, TEXT("Recv Fail: Header Not Recv"));
			continue;
		}

		// 데이터 받기
		BodyLength = NETWORK_ORDER16(BodyLength);
		RecvBuffer.SetNumUninitialized(BodyLength);
		TotalReadBytes = 0;
		bool IsBodyReady = false;
		while (TotalReadBytes < (int32)BodyLength)
		{
			int32 ReadBytes = 0;
			IsBodyReady = ServerSocket->Recv(RecvBuffer.GetData() + TotalReadBytes, BodyLength - TotalReadBytes, ReadBytes);
			if (!IsBodyReady || ReadBytes == 0)
			{
				IsBodyReady = false;
				break;
			}
			
			TotalReadBytes += ReadBytes;
		}

		// 데이터 준비 됐나?
		if (!IsBodyReady)
		{
			UE_LOG(LogTemp, Warning, TEXT("Recv Fail: Body Not Recv"));
			continue;
		}

		// Test Log
		auto Data = UserPacket::GetPacketData(RecvBuffer.GetData());
		if (Data->data_as_S2C_Login()->is_success())
			UE_LOG(LogTemp, Display, TEXT("Recv Success"));
		//

		RecvQueue.Enqueue(MoveTemp(RecvBuffer));
	}
	return uint32();
}

void FTCPRecvWorker::Stop()
{
	IsRunning = false;
}
