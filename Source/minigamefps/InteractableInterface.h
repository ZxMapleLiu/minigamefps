// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIGAMEFPS_API IInteractableInterface
{
	GENERATED_BODY()
public:
	virtual void React(AActor* OtherActor);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
