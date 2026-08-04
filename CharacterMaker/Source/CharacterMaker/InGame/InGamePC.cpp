// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePC.h"

void AInGamePC::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	BP_OnRepPlayerState();
}