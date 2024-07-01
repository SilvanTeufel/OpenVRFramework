// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h" 

// Sets default values
AVRUnitBase::AVRUnitBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create VR Root Component
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(RootComponent);

	// Create VR Camera Component
	VRCamera = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRRoot);
	VRCamera->MotionSource = FXRMotionControllerBase::HMDSourceId;

	// Initialize reference Z-Positions
	StandingZ = 180.f;  // Example value, adjust according to your setup
	KneelingZ = 90.f;   // Example value, adjust according to your setup
}

// Called when the game starts or when spawned
void AVRUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVRUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation();
	NormalizeHMDZPosition();
}

void AVRUnitBase::UpdateRotation()
{
	// Get the current orientation of the VR headset

	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDOrientation, HMDPosition);

	// Apply the HMD orientation to the character's rotation
	FRotator NewRotation = FRotator(0.f, HMDOrientation.Yaw, 0.f);
	SetActorRotation(NewRotation);
}

void AVRUnitBase::NormalizeHMDZPosition()
{
	if (StandingZ != KneelingZ)  // To avoid division by zero
	{
		NormalizedZ = (StandingZ - HMDPosition.Z) / (StandingZ - KneelingZ);
		NormalizedZ = FMath::Clamp(NormalizedZ, 0.f, 1.f);   // Ensure the value is between 0 and 1
	}
}
