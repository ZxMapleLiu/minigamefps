// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class USoundCue;
class UParticleEmitter;
UCLASS()
class MINIGAMEFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	//需要使用子弹进行开火
	virtual void Reload();
	virtual void EndReloading();
	FTimerHandle ReloadTimer;
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAutomatic;//是否为自动武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate;//射速(两发子弹之间的延迟)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponRecoil;//后坐力
	float CurrentRecoil;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int MaxAmmoInMag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int CurrentAmmoInMag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int MaxReservedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int CurrentReservedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")//需要使用子弹进行攻击
		bool bNeedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FString WeaponName;
	bool bIsReloading;
	//开火函数
	bool bIsFiring;
	//网格模型
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* NoAmmoSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* FireMuzzle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* ImpactEffect;
public:	
	FORCEINLINE USkeletalMeshComponent* GetMeshComp() const { return MeshComp; }
	virtual void SetFiring(bool firing);
};