// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMainWidget.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UTitleMainWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_CreateServer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Join;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_SetNickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_IPInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Nickname;

private:
	UFUNCTION()
	void OnClickedCreateServerBtn();

	UFUNCTION()
	void OnClickedJoinBtn();

	UFUNCTION()
	void OnClickedSetNicknameBtn();

	UFUNCTION()
	void CallFailUpdateNickname(const FString& InErrorMessage);
};
