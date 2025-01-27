// Copyright Epic Games, Inc. All Rights Reserved.

#include "minigamefpsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayPropertyComp.h"
#include "GameFramework/PawnMovementComponent.h"
//////////////////////////////////////////////////////////////////////////
// AminigamefpsCharacter

AminigamefpsCharacter::AminigamefpsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bIsSprinting = false;
	bIsFiring = false;
	bIsRealoading = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = 250;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FName HeadName("head");
	FollowCamera->SetupAttachment(GetMesh(), HeadName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

	GameplayPropertyComp = CreateDefaultSubobject<UGameplayPropertyComp>(TEXT("GameplayPropertyComp"));
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AminigamefpsCharacter::SwitchPosition()
{
	
	if (!(bIsCrouched))
	{
		this->BeginCrouch();
	}
	else
	{
		this->EndCrouch();
	}
	bIsCrouched = !bIsCrouched;
}

void AminigamefpsCharacter::BeginCrouch()
{
	Crouch();
}


void AminigamefpsCharacter::EndCrouch()
{
	UnCrouch();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AminigamefpsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AminigamefpsCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AminigamefpsCharacter::StopFiring);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AminigamefpsCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AminigamefpsCharacter::StopSprinting);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AminigamefpsCharacter::StartReloading);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AminigamefpsCharacter::Interact);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AminigamefpsCharacter::SwitchPosition);

	PlayerInputComponent->BindAxis("MoveForward", this, &AminigamefpsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AminigamefpsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AminigamefpsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AminigamefpsCharacter::LookUpAtRate);

}





void AminigamefpsCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = FollowCamera->GetComponentLocation();
	OutRotation = FollowCamera->GetComponentRotation();
}

void AminigamefpsCharacter::DropWeapon()
{
	if (WeaponSlot.Num()>1)
	{
		
		WeaponSlot[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		WeaponSlot[0]->SetOwner(nullptr);
		
	}
}

void AminigamefpsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AminigamefpsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AminigamefpsCharacter::Sprinting()
{
	if (!bIsRealoading && !bIsFiring)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 400;
		GetCharacterMovement()->MaxAcceleration *= 2;
	}
}

void AminigamefpsCharacter::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 250;
		GetCharacterMovement()->MaxAcceleration /= 2;
	}
	
}

void AminigamefpsCharacter::CheckForAutoFire()
{
	if (bIsFiring == true)
	{
		GetWorldTimerManager().ClearTimer(AutoWeaponFireTimerHandle);
		Fire();
	}
}

void AminigamefpsCharacter::StartFiring()
{
	bIsFiring = true;
	this->Fire();
}

void AminigamefpsCharacter::Fire()
{
	if (WeaponSlot.Num()>0 && !bIsSprinting && !bIsRealoading)
	{
		WeaponSlot[0]->Fire();
		WeaponSlot[0]->SetFiring(true);
		if (WeaponSlot[0]->bIsAutomatic == true)
		{
			GetWorldTimerManager().SetTimer(AutoWeaponFireTimerHandle, this, &AminigamefpsCharacter::CheckForAutoFire, WeaponSlot[0]->FireRate);
		}
	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("No Weapon"));
// 	}
}

void AminigamefpsCharacter::StopFiring()
{
	if (bIsFiring && WeaponSlot.Num()>0)
	{
		bIsFiring = false;
		WeaponSlot[0]->SetFiring(false);
	}
	
}

void AminigamefpsCharacter::Interact()
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Red, TEXT("Interact Pressed"));
	FHitResult Hit;
	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + (ShotDirection * 200);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
	IInteractableInterface* InteractObject = Cast<IInteractableInterface>(Hit.Actor);
	if (InteractObject)
	{
		InteractObject->React(this);
	}
}

void AminigamefpsCharacter::StartReloading()
{
	if (WeaponSlot.Num()>0 && bIsRealoading == false)
	{
		bIsRealoading = true;
		WeaponSlot[0]->Reload();
	}
}


void AminigamefpsCharacter::OnHealthChanged(UGameplayPropertyComp* GameplayComp, float Health, float Armor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		//�����ж�
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSlot[0]->SetOwner(nullptr);


		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}

void AminigamefpsCharacter::EndReloading()
{
	bIsRealoading = false;
}


void AminigamefpsCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameplayPropertyComp->OnHealthChanged.AddDynamic(this, &AminigamefpsCharacter::OnHealthChanged);
}

void AminigamefpsCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AminigamefpsCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && bIsSprinting==false)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}