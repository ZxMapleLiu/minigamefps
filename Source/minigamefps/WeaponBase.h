// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MINIGAMEFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AmmoInMag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ReservedAmmo;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//��Ҫʹ���ӵ����п���
	bool bNeedAmmo;
	//������
	UFUNCTION(BlueprintCallable,Category = "Weapon")
	virtual void Fire();
	//����ģ��
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* MeshComp;

public:	
	FORCEINLINE USkeletalMeshComponent* GetMeshComp() const { return MeshComp; }
};
