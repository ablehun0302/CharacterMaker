// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

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

USTRUCT(Atomic, BlueprintType)
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> Skill;
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
