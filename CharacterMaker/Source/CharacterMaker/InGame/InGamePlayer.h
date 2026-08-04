// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/Human.h"
#include "InGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API AInGamePlayer : public AHuman
{
	GENERATED_BODY()
	
protected:
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_OnRepPlayerState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_OnRepController();
};
