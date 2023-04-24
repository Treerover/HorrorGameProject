// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "TP_PickUpComponent.h"

// Sets default values
AFlashlight::AFlashlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Flashlight mesh
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	FlashlightMesh->SetupAttachment(RootComponent);

	// Flashlight light
	FlashlightLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightLight"));
	FlashlightLight->SetupAttachment(FlashlightMesh);

	// Flashlight sound
	FlashlightSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FlashlightSound"));
	FlashlightSound->SetupAttachment(FlashlightMesh);

	// Flashlight hitbox for pickup
	FlashlightHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("FlashlightHitbox"));
	FlashlightHitbox->SetupAttachment(FlashlightMesh);
	FlashlightHitbox->SetSphereRadius(50.0f);
    //Sets collision to on overlap
	FlashlightHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FlashlightHitbox->SetCollisionProfileName("OverlapAllDynamic");
	FlashlightHitbox->SetGenerateOverlapEvents(true);

	//On overlap function
	FlashlightHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFlashlight::OnOverlap);
	

	//Add pickupable tag
	Tags.Add(FName("Pickupable"));
}

void AFlashlight::ToggleFlashlight()
{
	//If the flashlight is on
	if (bIsOn == true)
	{
		//Turn the flashlight off
		bIsOn = false;
		if (FlashlightLight)
		{
			FlashlightLight->SetVisibility(false);
		}
	}
	//If the flashlight is off
	else if (bIsOn == false)
	{
		//Turn the flashlight on
		bIsOn = true;
		if (FlashlightLight)
		{
			FlashlightLight->SetVisibility(true);
		}
	}
}

// Called when the game starts or when spawned
void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlashlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void AFlashlight::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If the actor that overlaps is the player
	if (OtherActor->ActorHasTag("Player"))
	{
		//If the player is not holding anything
		if (Cast<AHorrorGameCharacter>(OtherActor)->GetHoldingFlashlight() == false)
		{
			//Pickup the flashlight
			Cast<AHorrorGameCharacter>(OtherActor)->SetHoldingFlashlight(true);

			//Assigns players flashlight class to this flashlight
			Cast<AHorrorGameCharacter>(OtherActor)->FlashlightActor = this;

			//Attach the flashlight to the player
			AttachToComponent(Cast<AHorrorGameCharacter>(OtherActor)->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "FlashlightSocket");
			//Moves the flashlight above the ground
			SetActorRelativeLocation(FVector(60.0f, 15.0f, 110.0f));
			
			//Disable the hitbox
			FlashlightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

