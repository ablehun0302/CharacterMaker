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
FCharacterStats UDataLibrary::AddCharacterStats(FCharacterStats A, FCharacterStats B)
{
	return A + B;
}

UDataLibrary::UDataLibrary()
{
}


