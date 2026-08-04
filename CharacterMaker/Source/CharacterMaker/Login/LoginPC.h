// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoginPC.generated.h"

class ULoginMainWidget;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API ALoginPC : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULoginMainWidget> WBP_LoginMainClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<ULoginMainWidget> WBP_LoginMainUI;

private:
	UFUNCTION()
	void CallSuccessSignIn();
};
