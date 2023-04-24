// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class HORRORGAME_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	// Hitbox for the object
	UPROPERTY(EditAnywhere)
		class UBoxComponent* HitBox;

	// The mesh for the object
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintImplementableEvent)
	void Interact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
