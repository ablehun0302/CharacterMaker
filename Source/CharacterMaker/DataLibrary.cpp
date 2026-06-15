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
FCharacterStats UDataLibrary::AddCharacterStats(FCharacterStats A, FCharacterStats B)
{
	return A + B;
}

FCharacterStats UDataLibrary::SubtractCharacterStats(FCharacterStats A, FCharacterStats B)
{
	return A - B;
}

FCharacterStats UDataLibrary::ClampCharacterStats(FCharacterStats InStats, FCharacterStats MaxCharacterStats)
{
	InStats.ClampMax(MaxCharacterStats);
	return InStats;
}

float UDataLibrary::GetHealthCharacterStats(FCharacterStats InStats)
{
	return InStats.Health;
}

float UDataLibrary::GetManaCharacterStats(FCharacterStats InStats)
{
	return InStats.Mana;
}

UDataLibrary::UDataLibrary()
{
	
}