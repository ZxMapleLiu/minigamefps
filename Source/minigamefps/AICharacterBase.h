// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "AICharacterBase.generated.h"

UCLASS()
class MINIGAMEFPS_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnHealthChanged(UGameplayPropertyComp* GameplayComp, float Health, float Armor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
		bool bDied;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "ture"))
		class UGameplayPropertyComp* GameplayPropertyComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		AWeaponBase* WeaponSlot;
	FORCEINLINE class UGameplayPropertyComp* GetGameplayPropertyComp() const { return GameplayPropertyComp; }

};
