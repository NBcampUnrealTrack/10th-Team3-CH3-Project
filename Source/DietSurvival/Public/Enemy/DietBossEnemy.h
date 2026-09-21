// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DietEnemyBase.h"
#include "DietBossEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);

UCLASS()
class DIETSURVIVAL_API ADietBossEnemy : public ADietEnemyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossDeath OnBossDeath;

	//Todo 보스 체력 설정 구현하기
protected:
	virtual void HandleDeath() override;
};
