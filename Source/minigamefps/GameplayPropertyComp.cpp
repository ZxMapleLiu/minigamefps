// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPropertyComp.h"
#include "GameFramework/Character.h"
// Sets default values for this component's properties
UGameplayPropertyComp::UGameplayPropertyComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	Health = 100;
	MaxArmor = 100;
	Armor = 100;
	bIsDead = false;
	// ...
}


// Called when the game starts
void UGameplayPropertyComp::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UGameplayPropertyComp::TakeDamage);
	}
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

void UGameplayPropertyComp::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Armor == 0)
	{
		if (Damage < Health)Health -= Damage;
		else
		{
			bIsDead = true;
			
		}
		if (bIsAbleToRecoverHealth == false)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimeHandler); //如果当前已经是没有在自动回血的状态，那么使上次设置的延迟3秒后自动回血失效
		}
		bIsAbleToRecoverHealth = false;
		GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &UGameplayPropertyComp::RecoverHealth, 3, false);
	}
	else
	{
		if (Damage <= Armor)
		{
			Armor -= Damage;
		}
		else
		{
			Damage -= Armor;
			Armor = 0;
			if (Damage < Health)Health -= Damage;
			else
			{
				bIsDead = true;
				
			}
			if (bIsAbleToRecoverHealth == false)
			{
				GetWorld()->GetTimerManager().ClearTimer(TimeHandler); //如果当前已经是没有在自动回血的状态，那么使上次设置的延迟3秒后自动回血失效
			}
			bIsAbleToRecoverHealth = false;
			GetWorld()->GetTimerManager().SetTimer(TimeHandler,this, &UGameplayPropertyComp::RecoverHealth, 3, false); //如果不循环最后一个参数不要填
		}
	}
}

void UGameplayPropertyComp::RecoverHealth()
{
	bIsAbleToRecoverHealth = true;
}

// Called every frame
void UGameplayPropertyComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsAbleToRecoverHealth == true && Health < MaxHealth && Health >0)
	{
		Health += 3;
	}
}

