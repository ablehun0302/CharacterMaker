// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkillPDA.h"

FPrimaryAssetId UBaseSkillPDA::GetPrimaryAssetId() const
{
	//Super::GetPrimaryAssetId();
	return FPrimaryAssetId(AssetType, GetFName());
}

bool UBaseSkillPDA::ExecuteSkillServer(APawn* Caster)
{
	if (Caster == nullptr)
	{
		return false;
	}

	return true;
}

bool UBaseSkillPDA::SpawnVisualEffect(APawn* Caster)
{
	if (Caster == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn Null"));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Pawn Location: %s"), *(Caster->GetActorLocation().ToString()));
	return true;
}
