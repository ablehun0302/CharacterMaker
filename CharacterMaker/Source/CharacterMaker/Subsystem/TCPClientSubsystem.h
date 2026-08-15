// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "TCPClientSubsystem.generated.h"

class FSocket;
class FTCPRecvWorker;
class FRunnableThread;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccessLogin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailLogin, const FString&, ErrorMsg);
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UTCPClientSubsystem : public UGameInstanceSubsystem , public FTickableGameObject
{
	GENERATED_BODY()
protected:
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

public:
	FOnSuccessLogin OnSuccessLogin;
	FOnFailLogin OnFailLogin;

private:
	FSocket* ServerSocket;

	FTCPRecvWorker* RecvWorker;
	FRunnableThread* RecvThread;
	TQueue<TArray<uint8>> RecvQueue;	// 수신한 데이터 -> 순차적 처리
	TArray<uint8> RecvBuffer;

public:
	void ConnectServer(const FString& Host, const int32 Port);
	void Disconnect();
	bool IsConnected();

	void SendLogin(const FString& InUID);

private:
	bool SendAll(const uint8* InBody, uint32 InBodyLength);
	void DispatchPacket();
};
