// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMainWidget.h"
#include "../Subsystem/AuthSubsystem.h"
#include "../Subsystem/TCPClientSubsystem.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 회원가입 시스템 불러오기
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	AuthSystem = GameInstance->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AuthSubsystem 없음"));
	}

	TCPSystem = GameInstance->GetSubsystem<UTCPClientSubsystem>();
	if (!TCPSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("TCPClientSubsystem 없음"));
	}

	// 버튼 클릭 이벤트
	if (Btn_SignUp)
	{
		Btn_SignUp->OnClicked.RemoveAll(this);
		Btn_SignUp->OnClicked.AddDynamic(this, &ULoginMainWidget::OnClickedSignUpBtn);
	}
	if (Btn_SignIn)
	{
		Btn_SignIn->OnClicked.RemoveAll(this);
		Btn_SignIn->OnClicked.AddDynamic(this, &ULoginMainWidget::OnClickedSignInBtn);
	}
}

void ULoginMainWidget::UpdateInfoText(const FString& InString)
{
	Text_Info->SetText(
		FText::FromString(InString)
	);
}

void ULoginMainWidget::ClearAllTextInput()
{
	TextBox_Email->SetText(FText::GetEmpty());
	TextBox_PW->SetText(FText::GetEmpty());
}

void ULoginMainWidget::OnClickedSignUpBtn()
{
	AuthSystem->OnSuccessSignUp.RemoveAll(this);
	AuthSystem->OnFailSignUp.RemoveAll(this);
	AuthSystem->OnSuccessSignUp.AddDynamic(this, &ULoginMainWidget::CallSuccessSignUp);
	AuthSystem->OnFailSignUp.AddDynamic(this, &ULoginMainWidget::CallFailSignUp);

	AuthSystem->SignUpEmail(TextBox_Email->GetText().ToString(), TextBox_PW->GetText().ToString());
}

void ULoginMainWidget::OnClickedSignInBtn()
{
	TCPSystem->OnFailLogin.RemoveAll(this);
	AuthSystem->OnFailVerifyPW.RemoveAll(this);
	TCPSystem->OnFailLogin.AddDynamic(this, &ULoginMainWidget::CallFailVerifyPW);
	AuthSystem->OnFailVerifyPW.AddDynamic(this, &ULoginMainWidget::CallFailVerifyPW);

	AuthSystem->SignInEmail(TextBox_Email->GetText().ToString(), TextBox_PW->GetText().ToString());
}

void ULoginMainWidget::CallSuccessSignUp(const FString& UID)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *UID);
	UpdateInfoText(TEXT("회원가입 완료"));
}

void ULoginMainWidget::CallFailSignUp(const FString& ErrorMsg)
{
	UpdateInfoText(FString::Printf(TEXT("회원가입 실패: %s"), *ErrorMsg));

	ClearAllTextInput();
}

void ULoginMainWidget::CallFailVerifyPW(const FString& ErrorMsg)
{
	UpdateInfoText(FString::Printf(TEXT("로그인 실패: %s"), *ErrorMsg));

	ClearAllTextInput();
}
