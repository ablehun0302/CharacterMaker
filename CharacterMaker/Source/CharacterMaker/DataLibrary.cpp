// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLibrary.h"

FCharacterStats& FCharacterStats::operator+(FCharacterStats OtherStats)
{
	Health += OtherStats.Health;
	Mana += OtherStats.Mana;
	Attack += OtherStats.Attack;
	MoveSpeed += OtherStats.MoveSpeed;
	JumpForce += OtherStats.JumpForce;

	return *this;
}
FCharacterStats& FCharacterStats::operator-(FCharacterStats OtherStats)
{
	Health -= OtherStats.Health;
	Mana -= OtherStats.Mana;
	Attack -= OtherStats.Attack;
	MoveSpeed -= OtherStats.MoveSpeed;
	JumpForce -= OtherStats.JumpForce;

	return *this;
}
void FCharacterStats::ClampMax(FCharacterStats MaxStats)
{
	Health = FMath::Clamp(Health, 0, MaxStats.Health);
	Mana = FMath::Clamp(Mana, 0, MaxStats.Mana);
	Attack = FMath::Clamp(Attack, 0, MaxStats.Attack);
	MoveSpeed = FMath::Clamp(MoveSpeed, 0, MaxStats.MoveSpeed);
	JumpForce = FMath::Clamp(JumpForce, 0, MaxStats.JumpForce);
}
int FCharacterStats::GetStatFromType(ECharacterStatType InType) const
{
	switch (InType)
	{
	case ECharacterStatType::Health:
		return Health;
		break;
	case ECharacterStatType::Mana:
		return Mana;
		break;
	case ECharacterStatType::Attack:
		return Attack;
		break;
	case ECharacterStatType::MoveSpeed:
		return MoveSpeed;
		break;
	case ECharacterStatType::JumpForce:
		return JumpForce;
		break;
	default:
		break;
	}

	// 타입이 아무것도 없다면
	return -1;
}
void FCharacterStats::SetStatFromType(ECharacterStatType InType, int InValue)
{
	switch (InType)
	{
	case ECharacterStatType::Health:
		Health += InValue;
		break;
	case ECharacterStatType::Mana:
		Mana += InValue;
		break;
	case ECharacterStatType::Attack:
		Attack += InValue;
		break;
	case ECharacterStatType::MoveSpeed:
		MoveSpeed += InValue;
		break;
	case ECharacterStatType::JumpForce:
		JumpForce += InValue;
		break;
	default:
		break;
	}
}
FCharacterStats UDataLibrary::AddCharacterStats(FCharacterStats A, FCharacterStats B)
{
	return A + B;
}

FCharacterStats UDataLibrary::SubtractCharacterStats(FCharacterStats A, FCharacterStats B)
{
	return A - B;
}

FCharacterStats UDataLibrary::ClampCharacterStats(FCharacterStats& InStats, const FCharacterStats& MaxCharacterStats)
{
	InStats.ClampMax(MaxCharacterStats);
	return InStats;
}

int UDataLibrary::GetCharacterStatFromType(const FCharacterStats& Target, ECharacterStatType InType)
{
	return Target.GetStatFromType(InType);
}

FCharacterStats& UDataLibrary::SetCharacterStatFromType(FCharacterStats& Target, ECharacterStatType InType, int InValue)
{
	Target.SetStatFromType(InType, InValue);
	return Target;
}

UDataLibrary::UDataLibrary()
{
	
}