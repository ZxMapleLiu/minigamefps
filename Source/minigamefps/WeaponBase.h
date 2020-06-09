// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class USoundCue;
class UParticleEmitter;

UCLASS()
class MINIGAMEFPS_API AWeaponBase : public AActor,public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	//��Ҫʹ���ӵ����п���
	virtual void Reload();
	virtual void EndReloading();
	FTimerHandle ReloadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAutomatic;//�Ƿ�Ϊ�Զ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate;//����(�����ӵ�֮����ӳ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponRecoil;//������
	float CurrentRecoil;
/*	UFUNCTION()*/
/*	void Overlap(AActor* OverlappedActor, AActor* OtherActor);*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int MaxAmmoInMag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")//��AI����Ҫ����ΪBlueprintReadWrite
		int CurrentAmmoInMag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int MaxReservedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int CurrentReservedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int WeaponHeadshotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAbleToHeadshot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")//��Ҫʹ���ӵ����й���
		bool bNeedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FString WeaponName;
	bool bIsReloading;
	//������
	bool bIsFiring;
	//����ģ��
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
	UParticleSystem* DefaultImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* FleshImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* HeadShotImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* GlassImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* ConcreteImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* MetalImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* WoodImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* StoneImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* ElectricImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* BrickImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* WaterImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* LeafImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* GrassImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* SnowImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* IceImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* ClothImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* SandImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* PaperImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UParticleSystem* ArmorImpactEffect;
public:	
	FORCEINLINE USkeletalMeshComponent* GetMeshComp() const { return MeshComp; }
	virtual void SetFiring(bool firing);
	virtual void SetOwner(AActor* NewOwner) override;
	virtual void React(AActor* OtherActor) override;
};