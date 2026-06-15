// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

class UNiagaraSystem;
class UTexture2D;

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
	FCharacterStats& operator-(FCharacterStats OtherStats);
	void ClampMax(FCharacterStats MaxStats);
};

UCLASS()
class CHARACTERMAKER_API UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UDataLibrary();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Add Character Stats", CompactNodeTitle = "+", Keywords = "+ add plus", ScriptMethod, ScriptMethodSelfReturn, ScriptOperator = "+;+="), Category = "Math|Data Library")
	static FCharacterStats AddCharacterStats(FCharacterStats A, FCharacterStats B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Subtract Character Stats", CompactNodeTitle = "-", Keywords = "- subtract minus", ScriptMethod, ScriptMethodSelfReturn, ScriptOperator = "-;-="), Category = "Math|Data Library")
	static FCharacterStats SubtractCharacterStats(FCharacterStats A, FCharacterStats B);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clamp Character Stats", Keywords = "clamp"), Category = "Math|Data Library")
	static FCharacterStats ClampCharacterStats(FCharacterStats InStats, FCharacterStats MaxCharacterStats);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Health from Stats", Keywords = "clamp"), Category = "Data|Data Library")
	static float GetHealthCharacterStats(FCharacterStats InStats);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Mana from Stats", Keywords = "clamp"), Category = "Data|Data Library")
	static float GetManaCharacterStats(FCharacterStats InStats);
};
