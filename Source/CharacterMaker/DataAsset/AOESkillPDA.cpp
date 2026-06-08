// Fill out your copyright notice in the Description page of Project Settings.


#include "AOESkillPDA.h"

void UAOESkillPDA::SpawnVisualEffect(APawn* Caster)
{
	Super::SpawnVisualEffect(Caster);

	if (Caster == nullptr)
	{
		return;
	}

	
}
