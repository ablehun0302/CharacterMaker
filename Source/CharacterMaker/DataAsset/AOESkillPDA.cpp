// Fill out your copyright notice in the Description page of Project Settings.


#include "AOESkillPDA.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UAOESkillPDA::ExecuteSkillServer(APawn* Caster)
{
	if (!Super::ExecuteSkillServer(Caster))
	{
		return false;
	}

	// 충돌 체크
	TArray<FHitResult> Hits;
	UKismetSystemLibrary::SphereTraceMultiByProfile(
		Caster,
		Caster->GetActorLocation(),
		Caster->GetActorLocation(),
		Radius,
		FName(TEXT("Pawn")),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		Hits,
		true
	);

	// 맞은 사람들 수치 변경
	for (auto &Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit!!!: %s"), *HitActor->GetName());
	}

	return true;
}

bool UAOESkillPDA::SpawnVisualEffect(APawn* Caster)
{
	if (!Super::SpawnVisualEffect(Caster))
	{
		return false;
	}

	ACharacter* Character = Cast<ACharacter>(Caster);
	float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector Location = Character->GetActorLocation() - FVector(0, 0, HalfHeight);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Caster,
		EffectParticle,
		Location
	);

	return true;
}
