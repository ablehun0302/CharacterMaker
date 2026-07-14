// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMainWidget.h"
#include "../Subsystem/AuthSubsystem.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_SignUp)
	{
		Btn_SignUp->OnClicked.RemoveAll(this);
		Btn_SignUp->OnClicked.AddDynamic(this, &ULoginMainWidget::OnClickedSignUpBtn);
	}
}

void ULoginMainWidget::ClearAllTextInput()
{
	TextBox_Email->SetText(FText::GetEmpty());
	TextBox_PW->SetText(FText::GetEmpty());
}

void ULoginMainWidget::OnClickedSignUpBtn()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	UAuthSubsystem* AuthSystem = GameInstance->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AuthSubsystem 없음"));
		return;
	}

	AuthSystem->OnSuccessSignUp.RemoveAll(this);
	AuthSystem->OnFailSignUp.RemoveAll(this);
	AuthSystem->OnSuccessSignUp.AddDynamic(this, &ULoginMainWidget::OnSuccessSignUp);
	AuthSystem->OnFailSignUp.AddDynamic(this, &ULoginMainWidget::OnFailSignUp);

	AuthSystem->SignUpEmail(TextBox_Email->GetText().ToString(), TextBox_PW->GetText().ToString());
}

void ULoginMainWidget::OnSuccessSignUp(const FString& UID)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *UID);
	Text_Info->SetText(FText::FromString(TEXT("회원가입 완료")));
}

void ULoginMainWidget::OnFailSignUp(const FString& ErrorMsg)
{
	Text_Info->SetText(
		FText::FromString(
			FString::Printf(TEXT("회원가입 실패: %s"), *ErrorMsg)
		)
	);

	ClearAllTextInput();
}
