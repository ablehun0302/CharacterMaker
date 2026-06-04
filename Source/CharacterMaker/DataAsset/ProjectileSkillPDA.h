// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillPDA.h"
#include "ProjectileSkillPDA.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UProjectileSkillPDA : public UBaseSkillPDA
{
	GENERATED_BODY()
public:
	virtual void ProcessSkill() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float BulletRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float LifeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float GravityScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UNiagaraSystem> BulletParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UNiagaraSystem> HitParticle;
};
