// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGM.h"
#include "../Subsystem/AuthSubsystem.h"

void ALoginGM::CallSuccessSignIn()
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	UAuthSubsystem* AuthSystem = GetGameInstance()->GetSubsystem<UAuthSubsystem>();
	if (!AuthSystem)
	{
		return;
	}

	AuthSystem
}
