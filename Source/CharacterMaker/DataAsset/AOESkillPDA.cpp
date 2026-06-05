// Fill out your copyright notice in the Description page of Project Settings.


#include "AOESkillPDA.h"

void UAOESkillPDA::ProcessSkill(APawn* Caster)
{
	Super::ProcessSkill(Caster);

	FString NameString = SkillInfo.SkillName.ToString();
	UE_LOG(LogTemp, Display, TEXT("AOE Skill Name: %s, Radius: %f"), *NameString, Radius);
}
