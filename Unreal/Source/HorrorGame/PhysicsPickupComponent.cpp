// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsPickupComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UPhysicsPickupComponent::UPhysicsPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhysicsPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Physics handle
	PhysicsHandle = NewObject<UPhysicsHandleComponent>(this, TEXT("PhysicsHandle"));
	PhysicsHandle->RegisterComponent();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}


}


// Called every frame
void UPhysicsPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPhysicsPickupComponent::Pickup()
{
	//If getpickupinsight returns an actor, set it to heldactor
	AActor* obj = GetPickupInSight();
	if (obj)
	{
		HeldActor = obj;
		bHolding = true;
		
		//Physics handle grab
		PhysicsHandle->GrabComponentAtLocationWithRotation(HeldActor->FindComponentByClass<UPrimitiveComponent>(), NAME_None, HeldActor->GetActorLocation(), HeldActor->GetActorRotation());
	}
}

void UPhysicsPickupComponent::Drop()
{
	PhysicsHandle->ReleaseComponent();
	bHolding = false;
	HeldActor = nullptr;
}

AActor* UPhysicsPickupComponent::GetPickupInSight()
{
	// If we're already holding an object, return
	if (HeldActor == nullptr)
	{
		// Get the player controller
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		// Get the player view point
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		// Draw a line from the player showing the pickup distance
		FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PickupDistance;
		DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red, false, 0.0f, 0.0f, 10.0f);

		// Raycast out to a certain distance (PickupDistance)
		FVector Start = PlayerViewPointLocation;
		FVector End = LineTraceEnd;
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		//Line trace (AKA raycast) out to reach distance
	
		if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic), CollisionQueryParams))
		{
			//Print hit result out
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
			//If actor has Pickup tag, and is not null, return the actor
			if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Pickup"))
			{
				return HitResult.GetActor();
			}
		}
		return nullptr;
	}
	else
	{
		return nullptr;
	}

}

