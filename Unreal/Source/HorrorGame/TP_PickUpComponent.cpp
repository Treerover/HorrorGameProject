// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{

}

void UTP_PickUpComponent::PickUp()
{
    if (!Owner)
    {
        Owner = GetOwner();
        Owner->SetActorEnableCollision(false);
        Owner->SetActorTickEnabled(false);
        Owner->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}

void UTP_PickUpComponent::Drop()
{
    if (Owner)
    {
        Owner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Owner->SetActorEnableCollision(true);
        Owner->SetActorTickEnabled(true);
        Owner = nullptr;
    }
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();


}

