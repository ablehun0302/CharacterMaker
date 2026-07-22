// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMainWidget.h"
#include "../Subsystem/AuthSubsystem.h"

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
		return;
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
	//AuthSystem->OnSuccessVerifyPW.RemoveAll(this);
	AuthSystem->OnFailVerifyPW.RemoveAll(this);
	//AuthSystem->OnSuccessVerifyPW.AddDynamic(this, &ULoginMainWidget::CallSuccessVerifyPW);
	AuthSystem->OnFailVerifyPW.AddDynamic(this, &ULoginMainWidget::CallFailVerifyPW);

	AuthSystem->SignInEmail(TextBox_Email->GetText().ToString(), TextBox_PW->GetText().ToString());
}

void ULoginMainWidget::CallSuccessSignUp(const FString& UID)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *UID);
	Text_Info->SetText(FText::FromString(TEXT("회원가입 완료")));
}

void ULoginMainWidget::CallFailSignUp(const FString& ErrorMsg)
{
	Text_Info->SetText(
		FText::FromString(
			FString::Printf(TEXT("회원가입 실패: %s"), *ErrorMsg)
		)
	);

	ClearAllTextInput();
}

void ULoginMainWidget::CallFailVerifyPW(const FString& ErrorMsg)
{
	UE_LOG(LogTemp, Display, TEXT("FailVerifyPW %s"), *ErrorMsg);

	Text_Info->SetText(
		FText::FromString(
			FString::Printf(TEXT("로그인 실패: %s"), *ErrorMsg)
		)
	);

	ClearAllTextInput();
}
