// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Http.h"
#include "AuthSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessSignUp, const FString&, UID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailSignUp, const FString&, ErrorMessage);
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UAuthSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:	// 콜백 함수 이후 서버->클라이언트 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSuccessSignUp OnSuccessSignUp;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFailSignUp OnFailSignUp;

private:
	const FString WebApiKey = TEXT("AIzaSyBHV9I9Yd2SIBnPIY2pkUk8vS_EnDCmhLQ");

public:
	void SignUpEmail(const FString& Email, const FString& PW);

private:	// 콜백 함수
	void CallSignUpNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);
};
