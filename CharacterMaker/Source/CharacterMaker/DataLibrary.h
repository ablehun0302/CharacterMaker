// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

class UNiagaraSystem;
class UTexture2D;

UENUM(BlueprintType)
enum class ECharacterStatType : uint8
{
	Health		UMETA(DisplayName = "Health"),
	Mana		UMETA(DisplayName = "Mana"),
	Attack		UMETA(DisplayName = "Attack"),
	MoveSpeed	UMETA(DisplayName = "Move Speed"),
	JumpForce	UMETA(DisplayName = "Jump Force")
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int JumpForce;

	FCharacterStats& operator+(FCharacterStats OtherStats);
	FCharacterStats& operator-(FCharacterStats OtherStats);
	void ClampMax(FCharacterStats MaxStats);
	int GetStatFromType(ECharacterStatType InType) const;
	void SetStatFromType(ECharacterStatType InType, int InValue);
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
	static FCharacterStats ClampCharacterStats(UPARAM(ref) FCharacterStats& InStats, const FCharacterStats& MaxCharacterStats);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Character Stat from Type", Keywords = "get"), Category = "Data|Data Library")
	static int GetCharacterStatFromType(const FCharacterStats& Target, ECharacterStatType InType);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Character Stat from Type", Keywords = "set"), Category = "Data|Data Library")
	static FCharacterStats& SetCharacterStatFromType(UPARAM(ref) FCharacterStats& Target, ECharacterStatType InType, int InValue);
};
