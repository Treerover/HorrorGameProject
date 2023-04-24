// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API URotateItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Rotates
	void Rotate(float DeltaTime);

	//Floats
	void Float(float DeltaTime);

	void SetRotateEnabled(bool bEnabled);
	void SetFloatEnabled(bool bEnabled);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Rotate speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
		float RotateSpeed = 1.0f;

	// Current rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
		FRotator CurrentRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
		bool bRotate = true;

	// Float up and down speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Float")
		float FloatSpeed = 1.0f;

	// Float up and down distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Float")
		float FloatDistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Float")
		bool bFloat = true;

	// Original location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Float")
		FVector OriginalLocation;

};
