// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DietBossEnemy.h"

void ADietBossEnemy::HandleDeath()
{
	if (bIsDead) return;

	Super::HandleDeath();
	OnBossDeath.Broadcast();
	Destroy();
}
