// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsPickupComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UPhysicsPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicsPickupComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Pickup();
	void Drop();

	//Gets pickup in sight
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		AActor* GetPickupInSight();

	UPROPERTY(EditAnywhere, Category = "Physics")
		class UPhysicsHandleComponent* PhysicsHandle = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Pickup")
		float PickupDistance = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		float PickupForce = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		float PickupOffset = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup")	
		FName PickupSocketName = "PickupSocket";

	UPROPERTY(EditAnywhere, Category = "Pickup")
		FName PickupButtonName = "Interact";

	UPROPERTY(EditAnywhere, Category = "Physics")
		float HandleStrength;


	UPROPERTY()
		AActor* HeldActor = nullptr;

	bool bHolding = false;


};
