// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkillPDA.h"

FPrimaryAssetId UBaseSkillPDA::GetPrimaryAssetId() const
{
	Super::GetPrimaryAssetId();
	return FPrimaryAssetId(AssetType, GetFName());
}

void UBaseSkillPDA::ProcessSkill()
{
	FString NameString = SkillInfo.SkillName.ToString();
	UE_LOG(LogTemp, Display, TEXT("Skill Name: %s"), *NameString);
}
