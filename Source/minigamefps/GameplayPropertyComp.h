// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayPropertyComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnHealthChangeSignature, UGameplayPropertyComp*, GameplayComp, float, Health, float, Armor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAMEFPS_API UGameplayPropertyComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayPropertyComp();
	FTimerHandle TimeHandler;//上一次受伤害的回复计时器
	UFUNCTION()
	void RecoverHealth();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		int MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		int Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		int MaxArmor; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		int Armor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	//喘气神功
	bool bIsAbleToRecoverHealth;
public:	
	int GetMaxHealth();
	int GetHealth();
	int GetMaxArmor();
	int GetArmor();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintAssignable,Category = "Events")
	FOnHealthChangeSignature OnHealthChanged;
};
