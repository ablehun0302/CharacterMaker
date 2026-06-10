// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "Projectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class CHARACTERMAKER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitProjectile(float InBulletRadius, float InSpeed, float InLifeSpan, float InGravityScale, UNiagaraSystem* InBulletParticle, UNiagaraSystem* InHitParticle);

	UFUNCTION()
	void ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> Particle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitParticle;
};
