// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPC.h"
#include "LoginMainWidget.h"

void ALoginPC::BeginPlay()
{
	Super::BeginPlay();

	if (WBP_LoginMainClass && IsLocalPlayerController())
	{
		WBP_LoginMainUI = CreateWidget<ULoginMainWidget>(this, WBP_LoginMainClass);

		if (WBP_LoginMainUI)
		{
			WBP_LoginMainUI->AddToViewport();
		}
	}

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
}
