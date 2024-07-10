// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "Components/CapsuleComponent.h" // Add this include
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AVRUnitBase::AVRUnitBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

	// Create VR Root Component
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent);


	// Create Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);
	//VRCamera->MotionSource = FXRMotionControllerBase::HMDSourceId;

	// Initialize reference Z-Positions
	StandingZ = 180.f;  // Example value, adjust according to your setup
	KneelingZ = 90.f;   // Example value, adjust according to your setup

	bUseControllerRotationYaw = true; // Ensure the character uses the controller's rotation for yaw

	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		CharMovement->bOrientRotationToMovement = false; // Disable orienting rotation to movement
		CharMovement->bUseControllerDesiredRotation = false; // Ensure controller desired rotation is not used
		CharMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // Set the rotation rate to zero if necessary
	}
	
}

// Called when the game starts or when spawned
void AVRUnitBase::BeginPlay()
{
	Super::BeginPlay();

	// Get initial HMD position and rotation
	FRotator HMDRotation;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);

	SetActorLocation(FVector(HMDPosition.X, HMDPosition.Y, GetActorLocation().Z));
	// Adjust the VRRoot position to align the VRCamera with the character's eye level
	//FVector CharacterEyeLevel = FVector(0.0f, 0.0f, -70.f); // Assuming StandingZ is the eye level height
	
	// Set the new position for VRRoot to align the camera at eye level
	//VRRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	
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
	// Get the current HMD orientation
	FRotator HMDRotation;
    
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);
	
	// Draw a debug line from the HMD position
	FVector HMDDirection = FRotationMatrix(HMDRotation).GetScaledAxis(EAxis::X);
	FVector HMDLineEnd = HMDPosition + (HMDDirection * 100.0f); // Adjust the length as needed
	DrawDebugLine(GetWorld(), HMDPosition, HMDLineEnd, FColor::Green, false, -1, 0, 5.0f);


	// Only use the yaw rotation for the character
	FRotator NewRotation =  FRotator(0.0f, HMDRotation.Yaw-90.f, 0.0f);
	
	// Set the character's rotation to match the HMD's yaw rotation
	//	SetActorRotation(NewRotation); // This is not working because HMD is PART of the Actor. How can we take this into Account?
	// Set the capsule component's rotation to match the HMD's yaw rotation
	GetMesh()->SetRelativeRotation(NewRotation);
}

/*
void AVRUnitBase::UpdateRotation()
{
	// Get the current HMD orientation
	FRotator HMDRotation;
	//FVector HMDPosition;
    
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);

	//FRotator CamRotation = Camera->GetRelativeRotation();
	// Only use the yaw rotation for the character
	FRotator NewRotation =  FRotator(0.0f, HMDRotation.Yaw, 0.0f);
	
	// Set the character's rotation to match the HMD's yaw rotation
	SetActorRotation(NewRotation);

	// Draw a debug line from the HMD position
	FVector HMDDirection = FRotationMatrix(HMDRotation).GetScaledAxis(EAxis::X);
	FVector HMDLineEnd = HMDPosition + (HMDDirection * 100.0f); // Adjust the length as needed
	DrawDebugLine(GetWorld(), HMDPosition, HMDLineEnd, FColor::Green, false, -1, 0, 5.0f);

	// Draw a debug line from the character's position
	FVector CharacterPosition = GetActorLocation();
	FVector CharacterDirection = FRotationMatrix(NewRotation).GetScaledAxis(EAxis::X);
	FVector CharacterLineEnd = CharacterPosition + (CharacterDirection * 100.0f); // Adjust the length as needed
	DrawDebugLine(GetWorld(), CharacterPosition, CharacterLineEnd, FColor::Blue, false, -1, 0, 5.0f);
}*/




void AVRUnitBase::MoveJoystick( float X, float Y)
{
	// Ensure that there is a valid movement component
	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		// Get the character's control rotation
		FRotator ActorRotation = GetActorRotation();

		FRotator HMDRotation;
    
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);

		// Only use the yaw rotation for movement
		HMDRotation.Pitch = 0.0f;
		HMDRotation.Roll = 0.0f;

		// Get the forward and right vectors based on the control rotation
		FVector ForwardDirection = FRotationMatrix(HMDRotation).GetScaledAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(HMDRotation).GetScaledAxis(EAxis::Y);

		// Calculate the movement direction based on the joystick input
		FVector MovementDirection = (ForwardDirection * Y) + (RightDirection * X);

		// Normalize the movement direction to ensure consistent movement speed
		MovementDirection.Normalize();

		// Apply the movement input to the character
		AddMovementInput(MovementDirection, 1.0f);
	}
}

void AVRUnitBase::NormalizeHMDZPosition()
{
	if (StandingZ != KneelingZ)  // To avoid division by zero
	{
		NormalizedZ = (StandingZ - HMDPosition.Z) / (StandingZ - KneelingZ);
		NormalizedZ = FMath::Clamp(NormalizedZ, 0.f, 1.f);   // Ensure the value is between 0 and 1
	}
}
