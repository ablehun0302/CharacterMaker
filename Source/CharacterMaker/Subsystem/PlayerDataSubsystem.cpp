// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDataSubsystem.h"
#include "AuthSubsystem.h"

void UPlayerDataSubsystem::UpdateNickname(const FString& InNickname)
{
	//AuthSubsystem 가져오기
	UAuthSubsystem* AuthSystem = GetGameInstance()->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		return;
	}

	// Request 설정
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString URL = FString::Printf(TEXT("https://firestore.asia-northeast3.rep.googleapis.com/v1/projects/rpg-project-c4596/databases/(default)/documents/user_info/%s"), *(AuthSystem->GetUID()));
	Request->SetURL(URL);
	Request->SetVerb(TEXT("PATCH"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *(AuthSystem->GetIdToken())));

	// Json 데이터 세팅
	FString ContentStr = FString::Printf(TEXT(R"({"fields":{"nickname": {"stringValue": "%s"}}})"), *InNickname);
	Request->SetContentAsString(ContentStr);

	// 응답 콜백 함수 바인딩
	Request->OnProcessRequestComplete().BindUObject(this, &UPlayerDataSubsystem::CallUpdateNickname);

	// 전송
	Request->ProcessRequest();
}

void UPlayerDataSubsystem::GetNickname()
{
	//AuthSubsystem 가져오기
	UAuthSubsystem* AuthSystem = GetGameInstance()->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		return;
	}

	// Request 설정
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString URL = FString::Printf(TEXT("https://firestore.asia-northeast3.rep.googleapis.com/v1/projects/rpg-project-c4596/databases/(default)/documents/user_info/%s"), *(AuthSystem->GetUID()));
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));

	// 응답 콜백 함수 바인딩
	Request->OnProcessRequestComplete().BindUObject(this, &UPlayerDataSubsystem::CallGetNickname);

	// 전송
	Request->ProcessRequest();
}

void UPlayerDataSubsystem::CallUpdateNickname(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	// 통신 실패
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		OnFailUpdateNickname.Broadcast(TEXT("통신 오류"));
		return;
	}

	// 수신받은 답변
	FString ResponseStr = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	// 파싱 실패
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnFailUpdateNickname.Broadcast(TEXT("파싱 실패"));
		return;
	}

	// 수신 오류
	if (Response->GetResponseCode() != 200)
	{
		const TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
		FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
		OnFailUpdateNickname.Broadcast(ErrorMessage);
		return;
	}
	const TSharedPtr<FJsonObject> FieldObject = JsonObject->GetObjectField(TEXT("fields"));
	const TSharedPtr<FJsonObject> NicknameObj = FieldObject->GetObjectField(TEXT("nickname"));
	NicknameObj->TryGetStringField(TEXT("stringValue"), Nickname);

	UE_LOG(LogTemp, Display, TEXT("---PlayerData CallUpdateNickname---\n%s"), *ResponseStr);

	OnSuccessUpdateNickname.Broadcast(Nickname);
}

void UPlayerDataSubsystem::CallGetNickname(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	// 통신 실패
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		//OnFailUpdateNickname.Broadcast(TEXT("통신 오류"));
		return;
	}

	// 수신받은 답변
	FString ResponseStr = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	// 파싱 실패
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//OnFailUpdateNickname.Broadcast(TEXT("파싱 실패"));
		return;
	}

	// 수신 오류
	if (Response->GetResponseCode() != 200)
	{
		/*const TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
		FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
		OnFailUpdateNickname.Broadcast(ErrorMessage);*/
		return;
	}
	const TSharedPtr<FJsonObject> FieldObject = JsonObject->GetObjectField(TEXT("fields"));
	const TSharedPtr<FJsonObject> NicknameObj = FieldObject->GetObjectField(TEXT("nickname"));
	NicknameObj->TryGetStringField(TEXT("stringValue"), Nickname);

	UE_LOG(LogTemp, Display, TEXT("---PlayerData CallGetNickname---\n%s"), *ResponseStr);

	OnSuccessUpdateNickname.Broadcast(Nickname);
}
