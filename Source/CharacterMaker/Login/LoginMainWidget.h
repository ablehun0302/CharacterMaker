// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginMainWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UAuthSubsystem;

/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API ULoginMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_PW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_SignUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_SignIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Info;

private:
	UAuthSubsystem* AuthSystem = nullptr;

private:
	void ClearAllTextInput();

	UFUNCTION()
	void OnClickedSignUpBtn();

	UFUNCTION()
	void OnClickedSignInBtn();

	// Client 콜백 함수
	UFUNCTION()
	void CallSuccessSignUp(const FString& UID);

	UFUNCTION()
	void CallFailSignUp(const FString& ErrorMsg);

	UFUNCTION()
	void CallSuccessVerifyPW(const FString& UID);

	UFUNCTION()
	void CallFailVerifyPW(const FString& ErrorMsg);
};
