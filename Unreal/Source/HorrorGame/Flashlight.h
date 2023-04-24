// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

UCLASS()
class HORRORGAME_API AFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashlight();

	// Flashlight mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
		class UStaticMeshComponent* FlashlightMesh;

	// Flashlight light
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
		class USpotLightComponent* FlashlightLight;

	// Flashlight sound
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
		class UAudioComponent* FlashlightSound;

	// Flashlight hitbox for pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
		class USphereComponent* FlashlightHitbox;


	//Turns the flashlight on/off
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
		void ToggleFlashlight();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//On overlap function
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Flashlight on/off
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
		bool bIsOn = true;
};
