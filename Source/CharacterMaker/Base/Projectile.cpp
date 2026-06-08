// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	RootComponent = Body;
	
	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Body);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::InitProjectile(float InBulletRadius, float InSpeed, float InLifeSpan, float InGravityScale, UNiagaraSystem* InBulletParticle)
{
	Body->SetSphereRadius(InBulletRadius);

	ProjectileMovement->InitialSpeed = InSpeed;
	ProjectileMovement->ProjectileGravityScale = InGravityScale;

	Particle->SetAsset(InBulletParticle);

	SetLifeSpan(InLifeSpan);
}

