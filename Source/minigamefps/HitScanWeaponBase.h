// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitScanWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMEFPS_API AHitScanWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	AHitScanWeaponBase();
protected:
	virtual void Fire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();
};
