// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Http.h"
#include "PlayerDataSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessUpdateNickname, const FString&, Nickname);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailUpdateNickname, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessGetNickname, const FString&, Nickname);
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

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSuccessGetNickname OnSuccessGetNickname;

private:
	FString Nickname;

public:
	void UpdateNickname(const FString& InNickname);

	void GetNickname();

private:
	void HandleDocumentResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully, TFunction<void (bool bSuccess, const FString& InData)> OnComplete);

	void CallGetNickname(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);
};
