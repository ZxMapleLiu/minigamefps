// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "InteractableInterface.h"
#include "minigamefpsCharacter.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	bIsReloading = false;
	bIsFiring = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() == nullptr)
	{
		this->GetMeshComp()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		this->GetMeshComp()->SetAllBodiesSimulatePhysics(true);
	}
	else
	{
		this->GetMeshComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->GetMeshComp()->SetAllBodiesSimulatePhysics(false);
	}
/*	OnActorBeginOverlap.AddDynamic(this, &AWeaponBase::Overlap);*/
}

void AWeaponBase::Tick(float DeltaTime)
{

}

void AWeaponBase::SetFiring(bool firing)
{
	bIsFiring = firing;
}


void AWeaponBase::SetOwner(AActor* NewOwner)
{
	if (NewOwner != nullptr)
	{
		Super::SetOwner(NewOwner);
		this->GetMeshComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->GetMeshComp()->SetAllBodiesSimulatePhysics(false);
	}
	else
	{
		Super::SetOwner(NewOwner);
		this->GetMeshComp()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		this->GetMeshComp()->SetAllBodiesSimulatePhysics(true);
	}
}

void AWeaponBase::React(AActor* OtherActor)
{
	AminigamefpsCharacter* PlayerChar = Cast<AminigamefpsCharacter>(OtherActor);
	if (PlayerChar)
	{
		PlayerChar->DropWeapon();
		SetOwner(PlayerChar);
		this->AttachToComponent(PlayerChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_socket"));
		PlayerChar->WeaponSlot = this;
	}
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

// void AWeaponBase::Overlap(AActor* OverlappedActor, AActor* OtherActor)
// {
// 	if (this->GetOwner() == nullptr)
// 	{
// 		AminigamefpsCharacter* OA = Cast<AminigamefpsCharacter>(OtherActor);
// 		if (OA && OA->WeaponSlot == nullptr)
// 		{
// 			this->SetOwner(OA);
// 			OA->WeaponSlot = this;
// 			AttachToComponent(OA->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_socket"));
// 			SetActorEnableCollision(false);
// 		}
// 	}
// }
