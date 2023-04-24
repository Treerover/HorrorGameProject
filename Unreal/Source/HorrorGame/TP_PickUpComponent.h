// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HorrorGameCharacter.h"
#include "TP_PickUpComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORGAME_API UTP_PickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTP_PickUpComponent();

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void PickUp();

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void Drop();

protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;


private:
	AActor* Owner;
};