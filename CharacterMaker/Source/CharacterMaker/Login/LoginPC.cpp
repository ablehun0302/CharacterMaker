// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPC.h"
#include "LoginMainWidget.h"
#include "../Subsystem/AuthSubsystem.h"

#include "Kismet/GameplayStatics.h"

void ALoginPC::BeginPlay()
{
	Super::BeginPlay();

	if (WBP_LoginMainClass && IsLocalPlayerController())
	{
		WBP_LoginMainUI = CreateWidget<ULoginMainWidget>(this, WBP_LoginMainClass);

		if (WBP_LoginMainUI)
		{
			WBP_LoginMainUI->AddToViewport();
		}
	}

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);

	// 로그인 성공 콜백 함수 바인딩
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("GM: No Game Instance"));
		return;
	}

	UAuthSubsystem* AuthSystem = GetGameInstance()->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GM: No Auth system"));
		return;
	}

	AuthSystem->OnSuccessVerifyPW.RemoveAll(this);
	AuthSystem->OnSuccessVerifyPW.AddDynamic(this, &ALoginPC::CallSuccessSignIn);
}

void ALoginPC::CallSuccessSignIn()
{
	if (IsLocalPlayerController())
	{
		UGameplayStatics::OpenLevel(this, TEXT("Title"));
	}
}
