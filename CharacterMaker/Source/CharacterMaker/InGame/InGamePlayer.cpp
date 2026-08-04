// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayer.h"

void AInGamePlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	BP_OnRepPlayerState();
}

void AInGamePlayer::OnRep_Controller()
{
	Super::OnRep_Controller();

	BP_OnRepController();
}
