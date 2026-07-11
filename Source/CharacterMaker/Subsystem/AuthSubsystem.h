// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Http.h"
#include "AuthSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UAuthSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	const FString WebApiKey = TEXT("AIzaSyBHV9I9Yd2SIBnPIY2pkUk8vS_EnDCmhLQ");

public:
	void SignUpEmail(const FString& Email, const FString& PW);

private:	// 콜백 함수
	void CallSignUpNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);
};
