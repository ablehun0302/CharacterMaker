// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Http.h"
#include "PlayerDataSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessUpdateNickname, const FString&, Nickname);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailUpdateNickname, const FString&, ErrorMessage);
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UPlayerDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSuccessUpdateNickname OnSuccessUpdateNickname;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnFailUpdateNickname OnFailUpdateNickname;

private:
	FString Nickname;

public:
	void UpdateNickname(const FString& InNickname);

	void GetNickname();

private:
	void CallUpdateNickname(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);

	void CallGetNickname(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);
};
