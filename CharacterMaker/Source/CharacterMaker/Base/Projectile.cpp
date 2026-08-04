// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "GameFramework/Character.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	
	if (Body)
	{
		Body->IgnoreActorWhenMoving(GetInstigator(), true);
		Body->OnComponentHit.AddDynamic(this, &AProjectile::ProcessComponentHit);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::InitProjectile(float InBulletRadius, float InSpeed, float InLifeSpan, float InGravityScale, UNiagaraSystem* InBulletParticle, UNiagaraSystem* InHitParticle, int InEffectValue)
{
	Body->SetSphereRadius(InBulletRadius);

	ProjectileMovement->InitialSpeed = InSpeed;
	ProjectileMovement->ProjectileGravityScale = InGravityScale;

	Particle->SetAsset(InBulletParticle);
	HitParticle = InHitParticle;

	EffectValue = InEffectValue;
	SetLifeSpan(InLifeSpan);
}

void AProjectile::ProcessComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 자신에게 충돌은 제외
	if (OtherActor == Owner)
	{
		return;
	}

	// 서버 충돌 처리
	if (Owner->HasAuthority())
	{
		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter != nullptr)
		{
			UGameplayStatics::ApplyDamage(OtherActor, EffectValue, nullptr, GetOwner(), nullptr);

			UE_LOG(LogTemp, Warning, TEXT("Hit!!!: %s"), *OtherActor->GetName());
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		HitParticle,
		this->GetActorLocation()
	);
	

	Destroy();
}

