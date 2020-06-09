// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeaponBase.h"
#include "minigamefps.h"
#include "Sound/SoundCue.h"
#include "minigamefpsCharacter.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/SkeletalMesh.h"
#include <typeinfo>
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AHitScanWeaponBase::AHitScanWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bNeedAmmo = true;
	bIsReloading = false;
	ReloadTime = 2.0f;
	bIsAutomatic = true;
	FireRate = 0.1f;
	bIsAbleToHeadshot = true;
	WeaponDamage = 25;
	WeaponHeadshotDamage = WeaponDamage * 4;
}
FRandomStream Stream;
void AHitScanWeaponBase::Fire()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, TEXT("WeaponFireFunctionTriggered"));
	
	
	APawn* WeaponOwner = Cast<APawn>(GetOwner());//����������ʱ��ǵ�����ӵ����
	//�ж�owner�Ƿ�Ϊ���
	if (WeaponOwner->GetClass()->IsChildOf(AminigamefpsCharacter::StaticClass()))
	{
		if (WeaponOwner && !bIsReloading)
		{

			if (bNeedAmmo)CurrentAmmoInMag -= 1;
			CurrentRecoil += WeaponRecoil;
			//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, TEXT("Fire"));
			USkeletalMeshComponent* Mesh = this->GetMeshComp();
			FVector EyeLocation;
			FRotator EyeRotation;
			WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			
			//���ſ�ǹЧ������Ч
			FVector GunFireLocation = Mesh->GetSocketByName("MuzzleLocation")->GetSocketLocation(Mesh);
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this,FireSound,GunFireLocation,EyeRotation);
				GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlaySound"));
			}
			if (FireMuzzle != nullptr)
			//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("OwnerDetected"));
			if (!bNeedAmmo || bNeedAmmo && CurrentAmmoInMag > 0)

			{
				if (bNeedAmmo)CurrentAmmoInMag -= 1;
				CurrentRecoil += WeaponRecoil;
				//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, TEXT("Fire"));
				USkeletalMeshComponent* Mesh = this->GetMeshComp();
				FVector EyeLocation;
				FRotator EyeRotation;
				WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

				//���ſ�ǹЧ������Ч
				FVector GunFireLocation = Mesh->GetSocketByName("MuzzleLocation")->GetSocketLocation(Mesh);
				if (FireSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GunFireLocation, EyeRotation);
					//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlaySound"));
				}
				if (FireMuzzle != NULL)
				{
					if (Mesh&&Mesh->GetSocketByName("MuzzleLocation"))
					{
						UGameplayStatics::SpawnEmitterAttached(FireMuzzle, Mesh, TEXT("MuzzleLocation"));
						//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlayMuzzle"));
					}
				}
				FVector ShotDirection = EyeRotation.Vector();
				if (WeaponOwner)
				{
					APlayerController* PC = Cast<APlayerController>(WeaponOwner->GetController());
					if (PC)
					{
						PC->ClientPlayCameraShake(FireCamShake);
					}
				}

				//������
				float RecoilXY = Stream.FRandRange(-CurrentRecoil, CurrentRecoil);
				FVector RecoilVector(RecoilXY, RecoilXY, CurrentRecoil * 10);
				FVector TraceEnd = EyeLocation + (ShotDirection * 10000) + RecoilVector;

				FCollisionQueryParams QueryParams;
				QueryParams.bReturnPhysicalMaterial = true;//������������
				QueryParams.AddIgnoredActor(WeaponOwner);
				QueryParams.AddIgnoredActor(this);
				QueryParams.bTraceComplex = true;
				FHitResult Hit;
				if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
				{
					EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
					UParticleSystem* SelectedEffect = nullptr;
					switch (SurfaceType)
					{
					case SurfaceType_Default:
						SelectedEffect = DefaultImpactEffect;
						break;
					case SURFACE_TYPE_METAL:
						SelectedEffect = MetalImpactEffect;
						break;
					case SURFACE_TYPE_WOOD:
						SelectedEffect = WoodImpactEffect;
						break;
					case SURFACE_TYPE_STONE:
						SelectedEffect = StoneImpactEffect;
						break;
					case SURFACE_TYPE_ELECTRIC:
						SelectedEffect = ElectricImpactEffect;
						break;
					case SURFACE_TYPE_CONCRETE:
						SelectedEffect = ConcreteImpactEffect;
						break;
					case SURFACE_TYPE_FLESH:
						SelectedEffect = FleshImpactEffect;
						break;
					case SURFACE_TYPE_BRICK:
						SelectedEffect = BrickImpactEffect;
						break;
					case SURFACE_TYPE_WATER:
						SelectedEffect = WaterImpactEffect;
						break;
					case SURFACE_TYPE_LEAF:
						SelectedEffect = LeafImpactEffect;
						break;
					case SURFACE_TYPE_GRASS:
						SelectedEffect = GrassImpactEffect;
						break;
					case SURFACE_TYPE_GLASS:
						SelectedEffect = GlassImpactEffect;
						break;
					case SURFACE_TYPE_SNOW:
						SelectedEffect = SnowImpactEffect;
						break;
					case SURFACE_TYPE_ICE:
						SelectedEffect = IceImpactEffect;
						break;
					case SURFACE_TYPE_CLOTH:
						SelectedEffect = ClothImpactEffect;
						break;
					case SurfaceType15:
						SelectedEffect = DefaultImpactEffect;
						break;
					case SURFACE_TYPE_SAND:
						SelectedEffect = SandImpactEffect;
						break;
					case SURFACE_TYPE_PAPER:
						SelectedEffect = PaperImpactEffect;
						break;
					case SURFACE_TYPE_ARMOR:
						SelectedEffect = ArmorImpactEffect;
						break;
					case SURFACE_TYPE_HS:
						SelectedEffect = HeadShotImpactEffect;
						break;
					default:
						SelectedEffect = DefaultImpactEffect;
						break;
					}
					if (SelectedEffect)
					{
						FVector HitPoint = Hit.ImpactPoint;
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, HitPoint, Hit.ImpactNormal.Rotation());
					}

					//@TODO:����Ŀ�꣬�����˺�
					AActor* HitActor = Hit.GetActor();
					if (SurfaceType == SURFACE_TYPE_HS)
					{
						UGameplayStatics::ApplyPointDamage(HitActor, WeaponHeadshotDamage, ShotDirection, Hit, WeaponOwner->GetInstigatorController(), this, DamageType);
					}
					else
					{
						UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage, ShotDirection, Hit, WeaponOwner->GetInstigatorController(), this, DamageType);

					}

					//ע�⣺��ɫ�ĳ����˺�������C++���޷���д����Ҫ����ͼ��ʵ��
					//�μ�Actor��TakenDamage������
				}
			}
			if (bNeedAmmo && CurrentAmmoInMag == 0)
			{
				if (NoAmmoSound)
					UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, this->GetActorLocation());
			}
		}
	}
	else
	//ownerΪAIʱ
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AI FIRE!"));
		if (WeaponOwner && !bIsReloading)
		{
			//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("OwnerDetected"));
			if (CurrentAmmoInMag > 0)
			{
				if (bNeedAmmo)CurrentAmmoInMag -= 1;
				CurrentRecoil += WeaponRecoil;
				//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, TEXT("Fire"));
				USkeletalMeshComponent* Mesh = this->GetMeshComp();
				FVector EyeLocation;
				FRotator EyeRotation;
				WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

				//���ſ�ǹЧ������Ч
				FVector GunFireLocation = Mesh->GetSocketByName("MuzzleLocation")->GetSocketLocation(Mesh);
				if (FireSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GunFireLocation, EyeRotation);
					//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlaySound"));
				}
				if (FireMuzzle != NULL)
				{
					if (Mesh&&Mesh->GetSocketByName("MuzzleLocation"))
					{
						UGameplayStatics::SpawnEmitterAttached(FireMuzzle, Mesh, TEXT("MuzzleLocation"));
						//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlayMuzzle"));
					}

				}
				FVector ShotDirection = EyeRotation.Vector();
				if (WeaponOwner)
				{
					APlayerController* PC = Cast<APlayerController>(WeaponOwner->GetController());
					if (PC)
					{
						PC->ClientPlayCameraShake(FireCamShake);
					}
				}

				//������
				float RecoilXY = Stream.FRandRange(-CurrentRecoil, CurrentRecoil);
				FVector RecoilVector(RecoilXY, RecoilXY, CurrentRecoil * 10);
				FVector TraceEnd = EyeLocation + (ShotDirection * 10000) + RecoilVector;

				FCollisionQueryParams QueryParams;
				QueryParams.bReturnPhysicalMaterial = true;//������������
				QueryParams.AddIgnoredActor(WeaponOwner);
				QueryParams.AddIgnoredActor(this);
				QueryParams.bTraceComplex = true;
				FHitResult Hit;
				//��ȡ���λ��
				APawn *playerPawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
				if (playerPawn)
				{
					FVector playerLocation = playerPawn->GetTargetLocation();
					if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, playerLocation, COLLISION_WEAPON, QueryParams))
					{

						DrawDebugLine(GetWorld(),EyeLocation, Hit.Location,FColor::Red);

						EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
						UParticleSystem* SelectedEffect = nullptr;
						switch (SurfaceType)
						{
						case SurfaceType_Default:
							SelectedEffect = DefaultImpactEffect;
							break;
						case SURFACE_TYPE_METAL:
							SelectedEffect = MetalImpactEffect;
							break;
						case SURFACE_TYPE_WOOD:
							SelectedEffect = WoodImpactEffect;
							break;
						case SURFACE_TYPE_STONE:
							SelectedEffect = StoneImpactEffect;
							break;
						case SURFACE_TYPE_ELECTRIC:
							SelectedEffect = ElectricImpactEffect;
							break;
						case SURFACE_TYPE_CONCRETE:
							SelectedEffect = ConcreteImpactEffect;
							break;
						case SURFACE_TYPE_FLESH:
							SelectedEffect = FleshImpactEffect;
							break;
						case SURFACE_TYPE_BRICK:
							SelectedEffect = BrickImpactEffect;
							break;
						case SURFACE_TYPE_WATER:
							SelectedEffect = WaterImpactEffect;
							break;
						case SURFACE_TYPE_LEAF:
							SelectedEffect = LeafImpactEffect;
							break;
						case SURFACE_TYPE_GRASS:
							SelectedEffect = GrassImpactEffect;
							break;
						case SURFACE_TYPE_GLASS:
							SelectedEffect = GlassImpactEffect;
							break;
						case SURFACE_TYPE_SNOW:
							SelectedEffect = SnowImpactEffect;
							break;
						case SURFACE_TYPE_ICE:
							SelectedEffect = IceImpactEffect;
							break;
						case SURFACE_TYPE_CLOTH:
							SelectedEffect = ClothImpactEffect;
							break;
						case SurfaceType15:
							SelectedEffect = DefaultImpactEffect;
							break;
						case SURFACE_TYPE_SAND:
							SelectedEffect = SandImpactEffect;
							break;
						case SURFACE_TYPE_PAPER:
							SelectedEffect = PaperImpactEffect;
							break;
						case SURFACE_TYPE_ARMOR:
							SelectedEffect = ArmorImpactEffect;
							break;
						case SURFACE_TYPE_HS:
							SelectedEffect = HeadShotImpactEffect;
							break;
						default:
							SelectedEffect = DefaultImpactEffect;
							break;
						}
						if (SelectedEffect)
						{
							FVector HitPoint = Hit.ImpactPoint;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, HitPoint, Hit.ImpactNormal.Rotation());
						}

						//@TODO:����Ŀ�꣬�����˺�
						AActor* HitActor = Hit.GetActor();
						if (SurfaceType == SURFACE_TYPE_HS)
						{
							//UGameplayStatics::ApplyPointDamage(HitActor, WeaponHeadshotDamage, ShotDirection, Hit, WeaponOwner->GetInstigatorController(), this, DamageType);
						}
						else
						{
							//UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage, ShotDirection, Hit, WeaponOwner->GetInstigatorController(), this, DamageType);

						}

						//ע�⣺��ɫ�ĳ����˺�������C++���޷���д����Ҫ����ͼ��ʵ��
						//�μ�Actor��TakenDamage������
					}

				}

			}
			if (bNeedAmmo && CurrentAmmoInMag == 0)
			{
				if (NoAmmoSound)
					UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, this->GetActorLocation());

			}
		}
			
	}
}

void AHitScanWeaponBase::Tick(float DeltaTime)
{
	if (CurrentRecoil > 0 && bIsFiring == false)
	{
		CurrentRecoil -= WeaponRecoil;
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
	else
	{
		Cast<AminigamefpsCharacter>(GetOwner())->EndReloading();
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

void AHitScanWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	
}


