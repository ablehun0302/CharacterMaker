// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkillPDA.h"

FPrimaryAssetId UBaseSkillPDA::GetPrimaryAssetId() const
{
	//Super::GetPrimaryAssetId();
	return FPrimaryAssetId(AssetType, GetFName());
}

void UBaseSkillPDA::ProcessSkill(APawn* Caster)
{
	if (Caster == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn Null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn Location: %s"), *(Caster->GetActorLocation().ToString()));
	}
}
