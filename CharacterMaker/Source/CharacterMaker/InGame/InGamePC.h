// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePC.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API AInGamePC : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_OnRepPlayerState();
};
