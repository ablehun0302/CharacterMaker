// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseSkillPDA.generated.h"

USTRUCT(BlueprintType)
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
	TObjectPtr<UTexture2D> SkillImage;
};
/**
 * 
 */
UCLASS()
class CHARACTERMAKER_API UBaseSkillPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AssetManager")
	FPrimaryAssetType AssetType;
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool ExecuteSkillServer(APawn* Caster);

	UFUNCTION(BlueprintCallable)
	virtual bool SpawnVisualEffect(APawn* Caster);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FSkillInfo SkillInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float CoolTime;
};
