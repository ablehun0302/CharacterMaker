// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMainWidget.h"
#include "../Subsystem/AuthSubsystem.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void ULoginMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_SignUp)
	{
		Btn_SignUp->OnClicked.RemoveAll(this);
		Btn_SignUp->OnClicked.AddDynamic(this, &ULoginMainWidget::OnClickedSignUpBtn);
	}
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

	AuthSystem->SignUpEmail(TextBox_Email->GetText().ToString(), TextBox_PW->GetText().ToString());
}
