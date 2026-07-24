// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMainWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterMaker/Subsystem/PlayerDataSubsystem.h"

void UTitleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_CreateServer)
	{
		Btn_CreateServer->OnClicked.AddDynamic(this, &UTitleMainWidget::OnClickedCreateServerBtn);
	}
	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &UTitleMainWidget::OnClickedJoinBtn);
	}
	if (Btn_SetNickname)
	{
		Btn_SetNickname->OnClicked.AddDynamic(this, &UTitleMainWidget::OnClickedSetNicknameBtn);
	}
}

void UTitleMainWidget::OnClickedCreateServerBtn()
{
	UGameplayStatics::OpenLevel(GetOwningPlayer(), "InGame01", true, TEXT("listen"));
}

void UTitleMainWidget::OnClickedJoinBtn()
{
	UGameplayStatics::OpenLevel(GetOwningPlayer(), FName(TextBox_IPInput->GetText().ToString()));
}

void UTitleMainWidget::OnClickedSetNicknameBtn()
{
	UPlayerDataSubsystem* PlayerDataSystem = GetGameInstance()->GetSubsystem<UPlayerDataSubsystem>();
	if (!PlayerDataSystem)
	{
		return;
	}

	PlayerDataSystem->OnSuccessUpdateNickname.RemoveAll(this);
	PlayerDataSystem->OnFailUpdateNickname.RemoveAll(this);
	PlayerDataSystem->OnSuccessUpdateNickname.AddDynamic(this, &UTitleMainWidget::CallSuccessUpdateNickname);
	PlayerDataSystem->OnFailUpdateNickname.AddDynamic(this, &UTitleMainWidget::CallFailUpdateNickname);

	PlayerDataSystem->UpdateNickname(TextBox_Nickname->GetText().ToString());
}

void UTitleMainWidget::CallSuccessUpdateNickname(const FString& InNickname)
{
	UE_LOG(LogTemp, Display, TEXT("New Nickname: %s"), *InNickname);
}

void UTitleMainWidget::CallFailUpdateNickname(const FString& InErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InErrorMessage);
}
