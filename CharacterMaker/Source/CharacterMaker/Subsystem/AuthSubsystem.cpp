// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TCPClientSubsystem.h"

const FString& UAuthSubsystem::GetIdToken()
{
	return IdToken;
}

const FString& UAuthSubsystem::GetRefreshToken()
{
	return RefreshToken;
}

const FString& UAuthSubsystem::GetUID()
{
	return UID;
}

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

void UAuthSubsystem::SignInEmail(const FString& Email, const FString& PW)
{
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	// Request 설정
	FString URL = FString::Printf(TEXT("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=%s"), *WebApiKey);
	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Json 데이터 세팅
	FString ContentStr = FString::Printf(TEXT(R"({"email":"%s","password":"%s","returnSecureToken":true})"), *Email, *PW);
	Request->SetContentAsString(ContentStr);

	// 응답 콜백 함수 바인딩
	Request->OnProcessRequestComplete().BindUObject(this, &UAuthSubsystem::CallVerifyPassword);

	// 전송
	Request->ProcessRequest();
}

void UAuthSubsystem::CallSignUpNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	// 통신 실패
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		OnFailSignUp.Broadcast(TEXT("통신 오류"));
		return;
	}

	// 수신받은 답변
	FString ResponseStr = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	// 파싱 실패
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnFailSignUp.Broadcast(TEXT("파싱 실패"));
		return;
	}

	// 수신 오류
	if (Response->GetResponseCode() != 200)
	{
		const TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
		FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
		OnFailSignUp.Broadcast(ErrorMessage);
		return;
	}
	
	JsonObject->TryGetStringField(TEXT("localId"), UID);
	UE_LOG(LogTemp, Display, TEXT("---Firebase Response---\n%s"), *ResponseStr);

	OnSuccessSignUp.Broadcast(UID);
}

void UAuthSubsystem::CallVerifyPassword(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	// 로그인 전에 TCP 서버 연결
	UTCPClientSubsystem* TCPSystem = GetGameInstance()->GetSubsystem<UTCPClientSubsystem>();
	if (TCPSystem && !TCPSystem->IsConnected())
	{
		TCPSystem->ConnectServer(TEXT("127.0.0.1"), 34567);
	}

	// TCP 서버 연결 없음
	if (!TCPSystem->IsConnected())
	{
		OnFailVerifyPW.Broadcast(TEXT("서버 통신 오류"));
		return;
	}
	
	// 통신 실패
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		OnFailVerifyPW.Broadcast(TEXT("통신 오류"));
		return;
	}

	// 수신받은 답변
	FString ResponseStr = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	// 파싱 실패
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnFailVerifyPW.Broadcast(TEXT("파싱 실패"));
		return;
	}

	// 수신 오류
	if (Response->GetResponseCode() != 200)
	{
		const TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
		FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
		OnFailVerifyPW.Broadcast(ErrorMessage);
		return;
	}

	JsonObject->TryGetStringField(TEXT("idToken"), IdToken);
	JsonObject->TryGetStringField(TEXT("refreshToken"), RefreshToken);
	JsonObject->TryGetStringField(TEXT("localId"), UID);
	JsonObject->TryGetStringField(TEXT("expiresIn"), ExpiresIn);

	TCPSystem->SendLogin(UID);
	UE_LOG(LogTemp, Display, TEXT("---Firebase Response---\n%s"), *ResponseStr);

	OnSuccessVerifyPW.Broadcast();
}
