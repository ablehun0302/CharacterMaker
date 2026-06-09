// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillPDA.h"
#include "AOESkillPDA.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UAOESkillPDA : public UBaseSkillPDA
{
	GENERATED_BODY()
public:
	UAOESkillPDA();
	virtual bool ExecuteSkillServer(APawn* Caster) override;

	virtual bool SpawnVisualEffect(APawn* Caster) override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float Radius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float EffectValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UNiagaraSystem> EffectParticle;
};
