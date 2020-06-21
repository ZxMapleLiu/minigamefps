// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameplayPropertyComp.h"
// Sets default values
AAICharacterBase::AAICharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameplayPropertyComp = CreateDefaultSubobject<UGameplayPropertyComp>(TEXT("GameplayPropertyComp"));
	bDied = false;
	bIsAttacked = false;
	bIsReloading = false;
	bFlag = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//��ί��
	GameplayPropertyComp->OnHealthChanged.AddDynamic(this, &AAICharacterBase::OnHealthChanged);
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacterBase::OnHealthChanged(UGameplayPropertyComp* GameplayComp, float Health, float Armor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	FString s = FString::SanitizeFloat(Damage);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, s);
	bIsAttacked = true;
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		//�����ж�
		GetMovementComponent()->StopMovementImmediately();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AI DIE"));
		DetachFromControllerPendingDestroy();
		//WeaponSlot->Destroy();
		WeaponSlot->SetOwner(nullptr);
		WeaponSlot->GetMeshComp()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SetLifeSpan(3.f);//TODO：根据死亡动画长度设置时间
	}
}

void AAICharacterBase::IfReload()
{
	if (WeaponSlot)
	{
		int TempAmmo = WeaponSlot->GetCurrentAmmoInMag();
		if (TempAmmo <= 0)
		{
			bIsReloading = true;
			return;
		}
	}

}

void AAICharacterBase::EndReloading()
{
	if (WeaponSlot)
	{
		bIsReloading = false;
	}
}

void AAICharacterBase::SetCurrentAmmo()
{
	if (WeaponSlot)
	{
		WeaponSlot->SetCurrentAmmoInMag();
	}
}

void AAICharacterBase::Reloading()
{
	bIsReloading = true;

}

void AAICharacterBase::Fire()
{
	if (WeaponSlot != NULL)
	{

		WeaponSlot->Fire();
	}
}