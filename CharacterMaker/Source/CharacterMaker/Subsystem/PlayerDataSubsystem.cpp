// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDataSubsystem.h"
#include "AuthSubsystem.h"

void UPlayerDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//AuthSubsystem 가져오기
	AuthSystem = Collection.InitializeDependency<UAuthSubsystem>();
	if (!AuthSystem)
	{
		return;
	}
}

const FString& UPlayerDataSubsystem::GetNicknameVar() const
{
	return Nickname;
}

void UPlayerDataSubsystem::UpdateNickname(const FString& InNickname)
{
	// Request 설정
	if (AuthSystem->GetUID().IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Null UID"));
		return;
	}

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
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
		{
			HandleDocumentResponse(Request, Response, bSuccess,
				[this](bool bSuccess, const FString& InData)
				{
					if (bSuccess)
					{
						OnSuccessUpdateNickname.Broadcast(InData);
					}
					else
					{
						OnFailUpdateNickname.Broadcast(InData);
					}
				});
		}
	);

	// 전송
	Request->ProcessRequest();
}

void UPlayerDataSubsystem::GetNickname()
{
	// Request 설정
	if (AuthSystem->GetUID().IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Null UID"));
		return;
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString URL = FString::Printf(TEXT("https://firestore.asia-northeast3.rep.googleapis.com/v1/projects/rpg-project-c4596/databases/(default)/documents/user_info/%s"), *(AuthSystem->GetUID()));
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *(AuthSystem->GetIdToken())));

	// 응답 콜백 함수 바인딩
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
		{
			HandleDocumentResponse(Request, Response, bSuccess,
				[this](bool bSuccess, const FString& InData)
				{
					if (bSuccess)
					{
						OnSuccessGetNickname.Broadcast(InData);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("GetNickname Error: %s"), *InData);
					}
				});
		}
	);

	// 전송
	Request->ProcessRequest();
}

void UPlayerDataSubsystem::HandleDocumentResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully, TFunction<void(bool bSuccess, const FString& InData)> OnComplete)
{
	// 통신 실패
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		OnComplete(false, TEXT("통신 오류"));
		return;
	}

	// 수신받은 답변
	FString ResponseStr = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	// 파싱 실패
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnComplete(false, TEXT("파싱 실패"));
		return;
	}

	// 수신 오류
	if (Response->GetResponseCode() != 200)
	{
		const TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
		FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
		OnComplete(false, ErrorMessage);
		return;
	}

	// 수신 성공시 닉네임 저장 및 이벤트 호출
	const TSharedPtr<FJsonObject> FieldObject = JsonObject->GetObjectField(TEXT("fields"));
	const TSharedPtr<FJsonObject> NicknameObj = FieldObject->GetObjectField(TEXT("nickname"));
	NicknameObj->TryGetStringField(TEXT("stringValue"), Nickname);

	UE_LOG(LogTemp, Display, TEXT("---PlayerData CallUpdateNickname---\n%s"), *ResponseStr);

	OnComplete(true, Nickname);
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

	OnSuccessGetNickname.Broadcast(Nickname);
}
