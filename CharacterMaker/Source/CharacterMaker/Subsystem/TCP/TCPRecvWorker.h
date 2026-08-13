// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;
/**
 * 
 */
class CHARACTERMAKER_API FTCPRecvWorker : public FRunnable
{
public:
	FTCPRecvWorker(FSocket* InServerSocket, TQueue<TArray<uint8>>& InRecvQueue);
	~FTCPRecvWorker();

public:
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FSocket* ServerSocket = nullptr;
	TQueue<TArray<uint8>>& RecvQueue;

	TArray<uint8> RecvBuffer;
	bool IsRunning = true;
};
