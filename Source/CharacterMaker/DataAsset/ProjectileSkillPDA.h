// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillPDA.h"
#include "ProjectileSkillPDA.generated.h"

class UNiagaraSystem;
class AProjectile;
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UProjectileSkillPDA : public UBaseSkillPDA
{
	GENERATED_BODY()
public:
	UProjectileSkillPDA();
	virtual bool ExecuteSkillServer(APawn* Caster) override;
	virtual bool SpawnVisualEffect(APawn* Caster) override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> BulletActorClass;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	int EffectValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float BulletRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float LifeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float GravityScale;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UNiagaraSystem> BulletParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UNiagaraSystem> HitParticle;
};
