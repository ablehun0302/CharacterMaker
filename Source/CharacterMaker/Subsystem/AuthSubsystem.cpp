// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthSubsystem.h"

void UAuthSubsystem::SignUpEmail(const FString& Email, const FString& PW)
{
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	// Request 설정
	FString URL = FString::Printf(TEXT("https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=%s"), *WebApiKey);
	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Json 데이터 세팅
	FString ContentStr = FString::Printf(TEXT(R"({"email":"%s","password":"%s","returnSecureToken":true})"), *Email, *PW);
	Request->SetContentAsString(ContentStr);

	// 응답 콜백 함수 바인딩
	Request->OnProcessRequestComplete().BindUObject(this, &UAuthSubsystem::CallSignUpNewUser);

	// 전송
	Request->ProcessRequest();
}

void UAuthSubsystem::CallSignUpNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Connect Error"));
		return;
	}

	if (Response->GetResponseCode() != 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("Response Error"));
		return;
	}
	
	FString ResponseStr = Response->GetContentAsString();
	UE_LOG(LogTemp, Display, TEXT("---Firebase Response---\n%s"), *ResponseStr);
}
