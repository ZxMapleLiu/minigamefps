// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	bIsReloading = false;
	bIsFiring = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{

}

void AWeaponBase::SetFiring(bool firing)
{
	bIsFiring = firing;
}

void AWeaponBase::Fire()
{

}

void AWeaponBase::Reload()
{

}

void AWeaponBase::EndReloading()
{

}
