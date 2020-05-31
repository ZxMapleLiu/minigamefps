// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeaponBase.h"
#include "Sound/SoundCue.h"
#include "minigamefpsCharacter.h"
#include "Particles/ParticleEmitter.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

AHitScanWeaponBase::AHitScanWeaponBase()
{
	bNeedAmmo = true;
	bIsReloading = false;
	ReloadTime = 2.0f;
	bIsAutomatic = true;
	FireRate = 0.1f;
}

void AHitScanWeaponBase::Fire()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, TEXT("WeaponFireFunctionTriggered"));
	AActor* WeaponOwner = GetOwner();//����������ʱ��ǵ�����ӵ����
	if (WeaponOwner && !bIsReloading)
	{
		//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("OwnerDetected"));
		if (!bNeedAmmo || bNeedAmmo && CurrentAmmoInMag>0)
		{
			if (bNeedAmmo)CurrentAmmoInMag -= 1;
			//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, TEXT("Fire"));
			USkeletalMeshComponent* Mesh = this->GetMeshComp();
			FVector EyeLocation;
			FRotator EyeRotation;
			WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			//���ſ�ǹЧ������Ч
			FVector GunFireLocation = Mesh->GetSocketByName("MuzzleLocation")->GetSocketLocation(Mesh);
			if (FireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this,FireSound,GunFireLocation,EyeRotation);
				//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlaySound"));
			}
			if (FireMuzzle != NULL)
			{
				if (Mesh&&Mesh->GetSocketByName("MuzzleLocation"))
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, FireMuzzle, GunFireLocation, EyeRotation);
					//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlayMuzzle"));
				}
				
			}
			FVector ShotDirection = EyeRotation.Vector();

			FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(WeaponOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				//@TODO:����Ŀ�꣬�����˺�
				AActor* HitActor = Hit.GetActor();

				UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, WeaponOwner->GetInstigatorController(), this, DamageType);
				//ע�⣺��ɫ�ĳ����˺�������C++���޷���д����Ҫ����ͼ��ʵ��
				//�μ�Actor��TakenDamage������
			}
		}
		if (bNeedAmmo && CurrentAmmoInMag == 0)
		{
			if(NoAmmoSound)
			UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, this->GetActorLocation());
		}
	}
}

void AHitScanWeaponBase::Reload()
{
	if (bNeedAmmo && CurrentAmmoInMag < MaxAmmoInMag)
	{
		bIsReloading = true;
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, this->GetActorLocation());
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AHitScanWeaponBase::EndReloading, ReloadTime);
	}
}
void AHitScanWeaponBase::EndReloading()
{
	if (CurrentReservedAmmo + CurrentAmmoInMag >= MaxAmmoInMag)
	{
		CurrentReservedAmmo += (CurrentAmmoInMag - MaxAmmoInMag);
		CurrentAmmoInMag = MaxAmmoInMag;
	}
	else if (CurrentReservedAmmo > 0)
	{
		CurrentAmmoInMag += CurrentReservedAmmo;
		CurrentReservedAmmo = 0;
	}
	Cast<AminigamefpsCharacter>(GetOwner())->EndReloading();
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	bIsReloading = false;
}