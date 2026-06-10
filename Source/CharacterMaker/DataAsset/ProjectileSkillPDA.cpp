// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillPDA.h"
#include "Components/ArrowComponent.h"
#include "CharacterMaker/Base/Human.h"
#include "CharacterMaker/Base/Projectile.h"

UProjectileSkillPDA::UProjectileSkillPDA()
{
	SkillInfo.SkillType = ESkillType::Projectile;
	BulletActorClass = AProjectile::StaticClass();
}

bool UProjectileSkillPDA::SpawnVisualEffect(APawn* Caster)
{
	if (!Super::SpawnVisualEffect(Caster))
	{
		return false;
	}

	AHuman* Character = Cast<AHuman>(Caster);

	// 투사체 생성 보류
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	auto SpawnedActor = Caster->GetWorld()->SpawnActorDeferred<AProjectile>(
		BulletActorClass,
		Character->GetProjectileSpawnPoint()->GetComponentTransform(),
		Caster,
		Caster
	);

	// 투사체 속성 초기화
	if (SpawnedActor == nullptr)
	{
		return false;
	}
	SpawnedActor->InitProjectile(BulletRadius, Speed, LifeSpan, GravityScale, BulletParticle, HitParticle);

	// 투사체 최종 생성
	SpawnedActor->FinishSpawning(Character->GetProjectileSpawnPoint()->GetComponentTransform());

	return true;
}
