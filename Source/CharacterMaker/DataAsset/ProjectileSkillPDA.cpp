// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillPDA.h"
#include "Components/ArrowComponent.h"
#include "../Base/Human.h"

void UProjectileSkillPDA::ProcessSkill(APawn* Caster)
{
	Super::ProcessSkill(Caster);

	if (Caster == nullptr)
	{
		return;
	}

	AHuman* Character = Cast<AHuman>(Caster);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	Caster->GetWorld()->SpawnActor<AActor>(BulletActorClass, Character->GetProjectileSpawnPoint()->GetComponentTransform(), SpawnParams);
}
