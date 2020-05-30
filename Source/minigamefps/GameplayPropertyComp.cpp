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
	// ...
}


// Called when the game starts
void UGameplayPropertyComp::BeginPlay()
{
	Super::BeginPlay();
	// ...
	TakeDamage(150);
	
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

void UGameplayPropertyComp::TakeDamage(int value)
{
	if (Armor == 0)
	{
		if (value < Health)Health -= value;
		else
		{
			//TODO：角色死亡，传递信息给OwnerCharacter
			ACharacter* CompOwner = Cast<ACharacter>(GetOwner());
		}
		bIsAbleToRecoverHealth = false;
	}
	else
	{
		if (value <= Armor)
		{
			Armor -= value;
		}
		else
		{
			value -= Armor;
			Armor = 0;
			if (value < Health)Health -= value;
			else
			{
				//TODO：角色死亡，传递信息给OwnerCharacter
				ACharacter* CompOwner = Cast<ACharacter>(GetOwner());
			}
			bIsAbleToRecoverHealth = false;
			//TODO：设置延迟喘气回血
			GetWorld()->GetTimerManager().SetTimer(Handler,this, &UGameplayPropertyComp::RecoverHealth, 3, false, 0);
		}
	}
}

void UGameplayPropertyComp::RecoverHealth()
{
	bIsAbleToRecoverHealth = true;
	if (bIsAbleToRecoverHealth)
	{
		return;
	}
}

// Called every frame
void UGameplayPropertyComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//TODO:写喘气神功的逻辑
	// ...
	if (bIsAbleToRecoverHealth == true && Health < MaxHealth && Health >0)
	{
		Health += 3;
	}
}

