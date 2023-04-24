// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateItemComponent.h"

// Sets default values for this component's properties
URotateItemComponent::URotateItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	RotateSpeed = 40.0f;
	FloatSpeed = 5.0f;
	FloatDistance = 3.0f;

}


// Called when the game starts
void URotateItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentRotation = GetOwner()->GetActorRotation();
	OriginalLocation = GetOwner()->GetActorLocation();
}

void URotateItemComponent::Rotate(float DeltaTime)
{
	FRotator NewRotation = GetOwner()->GetActorRotation() + FRotator(0, RotateSpeed * DeltaTime, 0);
	GetOwner()->SetActorRotation(NewRotation);
}

void URotateItemComponent::Float(float DeltaTime)
{
	//Implement later 
}

void URotateItemComponent::SetRotateEnabled(bool bEnabled)
{
	bRotate = bEnabled;
}

void URotateItemComponent::SetFloatEnabled(bool bEnabled)
{
	bFloat = bEnabled;
}


// Called every frame
void URotateItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If rotate is enabled
	if (bRotate)
	{
		Rotate(DeltaTime);
	}

	//If float is enabled
	if (bFloat)
	{
		Float(DeltaTime);
	}

	//If the parent actor is attached to the player, dont rotate or float.
	if (GetOwner()->GetAttachParentActor() != nullptr)
	{
		SetRotateEnabled(false);
		SetFloatEnabled(false);
	}


}

