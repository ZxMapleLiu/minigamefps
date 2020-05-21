// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayPropertyComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAMEFPS_API UGameplayPropertyComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayPropertyComp();

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
public:	
	int GetMaxHealth();
	int GetHealth();
	int GetMaxArmor();
	int GetArmor();
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
