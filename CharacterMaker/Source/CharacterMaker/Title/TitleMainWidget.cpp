// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMainWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
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

	// 플레이어의 닉네임 가져오기
	UPlayerDataSubsystem* PlayerDataSystem = GetGameInstance()->GetSubsystem<UPlayerDataSubsystem>();
	if (!PlayerDataSystem)
	{
		return;
	}

	PlayerDataSystem->GetNickname();
	PlayerDataSystem->OnSuccessGetNickname.AddDynamic(this, &UTitleMainWidget::CallSuccessGetNickname);
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
	Text_NicknameInfo->SetText(FText::FromString(TEXT("닉네임 변경 완료")));
}

void UTitleMainWidget::CallFailUpdateNickname(const FString& InErrorMessage)
{
	Text_NicknameInfo->SetText(FText::FromString(FString::Printf(TEXT("닉네임 변경 실패: %s"), *InErrorMessage)));
	TextBox_Nickname->SetText(FText::GetEmpty());
}

void UTitleMainWidget::CallSuccessGetNickname(const FString& InNickname)
{
	if (TextBox_Nickname)
	{
		TextBox_Nickname->SetText(FText::FromString(InNickname));
	}
}
