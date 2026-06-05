// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillPDA.h"

void UProjectileSkillPDA::ProcessSkill(APawn* Caster)
{
	Super::ProcessSkill(Caster);

	FString NameString = SkillInfo.SkillName.ToString();
	UE_LOG(LogTemp, Display, TEXT("Projectile Skill Name: %s, BulletRadius: %f"), *NameString, BulletRadius);
}
