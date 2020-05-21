// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPropertyComp.h"

// Sets default values for this component's properties
UGameplayPropertyComp::UGameplayPropertyComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	Health = 100;
	MaxArmor = 100;
	Armor = 100;
	// ...
}


// Called when the game starts
void UGameplayPropertyComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int UGameplayPropertyComp::GetMaxHealth()
{
	return MaxHealth;
}

int UGameplayPropertyComp::GetHealth()
{
	return Health;
}

int UGameplayPropertyComp::GetMaxArmor()
{
	return MaxArmor;
}

int UGameplayPropertyComp::GetArmor()
{
	return Armor;
}

// Called every frame
// void UGameplayPropertyComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 
// 	// ...
// }

