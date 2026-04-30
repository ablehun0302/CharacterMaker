// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

class UNiagaraSystem;

USTRUCT(Atomic, BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpForce;

	FCharacterStats& operator+(FCharacterStats OtherStats);
};

USTRUCT(BlueprintType)
struct FBaseSkillData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;
};

USTRUCT(BlueprintType)
struct FAOESkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseSkillData BaseSkillData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue;
};

USTRUCT(BlueprintType)
struct FProjectileSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseSkillData BaseSkillData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> BulletParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> HitParticle;
};

USTRUCT(Atomic, BlueprintType)
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;
};

USTRUCT(Atomic, BlueprintType)
struct FAOESkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue;
};

UCLASS()
class CHARACTERMAKER_API UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UDataLibrary();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Add Character Stats", CompactNodeTitle = "+", Keywords = "+ add plus", ScriptMethod, ScriptMethodSelfReturn, ScriptOperator = "+;+="), Category = "Math|Data Library")
	static FCharacterStats AddCharacterStats(FCharacterStats A, FCharacterStats B);
};
