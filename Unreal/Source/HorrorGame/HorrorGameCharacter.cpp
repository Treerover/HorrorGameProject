// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorrorGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/InputSettings.h"
#include "Flashlight.h"
#include <Kismet/GameplayStatics.h>
#include "InteractableObject.h"
#include "PhysicsPickupComponent.h"

//////////////////////////////////////////////////////////////////////////
// AHorrorGameCharacter

AHorrorGameCharacter::AHorrorGameCharacter()
{	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Pickup component
	PickupComponent = CreateDefaultSubobject<UPhysicsPickupComponent>(TEXT("PickupComponent"));

	Tags.Add("Player");
}

void AHorrorGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AHorrorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Look);

		//Flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Flashlight);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Interact);

		//Pickup
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Pickup);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::CrouchPressed);
	}
}

bool AHorrorGameCharacter::GetHoldingFlashlight()
{
	return bHoldingFlashlight;
}

void AHorrorGameCharacter::SetHoldingFlashlight(bool bHolding)
{
	bHoldingFlashlight = bHolding;
}


void AHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHorrorGameCharacter::Flashlight(const FInputActionValue& Value)
{
	//Turns the flashlight on and off
	// Casts to the Flashlight class and calls the ToggleFlashlight function

	if (bHoldingFlashlight)
	{
		//Finds the flashlight in the scene
		AFlashlight* Flashlight = Cast<AFlashlight>(FlashlightActor);
		Flashlight->ToggleFlashlight();
	}
}

void AHorrorGameCharacter::Interact(const FInputActionValue& Value)
{
	// Get the player's camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * InteractDistance);

	// Perform the raycast
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore the player character
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel1, QueryParams);

	// Check if the raycast hit an interactable object
	if (bHit)
	{
		// Cast the hit actor to an interactable object
		AInteractableObject* InteractableObject = Cast<AInteractableObject>(HitResult.GetActor());
		if (InteractableObject)
		{
			// Call the Interact function on the interactable object
			InteractableObject->Interact();
		}
	}

}

void AHorrorGameCharacter::Pickup(const FInputActionValue& Value)
{
	// Calls pickup on the physics pickup component
	PickupComponent->Pickup();
}

void AHorrorGameCharacter::Drop(const FInputActionValue& Value)
{
	// Calls drop on the physics pickup component
	PickupComponent->Drop();
}

void AHorrorGameCharacter::CrouchPressed(const FInputActionValue& Value)
{
	//If player is crouching already
	if (bIsCrouching)
	{
		//Stand up
		UnCrouch();
		bIsCrouching = false;
	}
	//Otherwise, start crouching
	else
	{
		//Crouch
		Crouch();
		bIsCrouching = true;
	}
}

