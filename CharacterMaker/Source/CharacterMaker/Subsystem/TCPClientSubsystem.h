// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TCPClientSubsystem.generated.h"

class FSocket;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UTCPClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	virtual void Deinitialize() override;

private:
	FSocket* ServerSocket;

public:
	void ConnectServer(const FString& Host, const int32 Port);
	void Disconnect();
	bool IsConnected();

	void SendLogin(const FString& InIdToken, const FString& InExpiresIn);

private:
	bool SendAll(const uint8* InBody, uint32 InBodyLength);
};
