// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeaponBase.h"
#include "Kismet/GameplayStatics.h"

void AHitScanWeaponBase::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

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

			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f,ShotDirection,Hit, WeaponOwner->GetInstigatorController(), this, DamageType);
			//ע�⣺��ɫ�ĳ����˺�������C++���޷���д����Ҫ����ͼ��ʵ��
			//�μ�Actor��TakenDamage������
		}
	}
}

void AHitScanWeaponBase::Reload()
{
}
