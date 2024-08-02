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
	GetMesh()->SetupAttachment(VROrigin);
	//Camera->SetMobility(EComponentMobility::Movable);
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

	// Create motion controller components
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->MotionSource = FName("LeftGrip");
	LeftMotionController->SetupAttachment(VROrigin);
	LeftMotionController->ComponentTags.Add(FName("LeftHand"));

	
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->MotionSource = FName("RightGrip");
	RightMotionController->SetupAttachment(VROrigin);
	RightMotionController->ComponentTags.Add(FName("RightHand"));
}

// Called when the game starts or when spawned
void AVRUnitBase::BeginPlay()
{
	Super::BeginPlay();

	// Get initial HMD position and rotation
	//SetRotationAndPosition();
	//SetActorLocation(FVector(HMDPosition.X, HMDPosition.Y, GetActorLocation().Z));
	// Start the timer to switch the movement hand position every interval
	//GetWorld()->GetTimerManager().SetTimer(MovementHandTimerHandle, this, &AVRUnitBase::SwitchMovementHandPosition, MovementHandInterval, true);
	// Adjust the VRRoot position to align the VRCamera with the character's eye level
	//FVector CharacterEyeLevel = FVector(0.0f, 0.0f, -70.f); // Assuming StandingZ is the eye level height
	
	// Set the new position for VRRoot to align the camera at eye level
	//VRRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	// Null the actor with the HMD location
	NullActorWithHMDLocation();
	//CalibrateMotionControllers();

}

// Called every frame
void AVRUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetRotationAndPosition();
	UpdateRotation();
	NormalizeHMDZPosition();
	
	CalculateHeadLocation(DeltaTime);
	CalculateHandLocation(DeltaTime);
	//VROrigin->SetRelativeLocation(FVector(0.f, 0.f, -120.f));

	SetActorToHMDChange(DeltaTime);

	//SetVROriginLocation();

}

void AVRUnitBase::SetRotationAndPosition()
{
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);
	Camera->SetWorldRotation(HMDRotation);
}
void AVRUnitBase::SetVROriginLocation()
{
	FVector ActorLocation = GetActorLocation();
	VROrigin->SetWorldLocation(FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z-120.f));
}

void AVRUnitBase::UpdateRotation()
{

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
	//SetActorRotation(NewRotation);
	//SetActorRelativeRotation(NewRotation);
}

void AVRUnitBase::CalculateHeadLocation(float DeltaTime)
{
	// Convert rotation to quaternion for easier manipulation
	FQuat HMDQuat = FQuat(HMDRotation);

	// Calculate forward vector based on pitch and roll
	FVector ForwardVector = HMDQuat.GetForwardVector();
	FVector ActorLocation = GetActorLocation();
	// Use forward vector components for X and Y (ignore Z to make it independent)
	float XComponent = HMDRotation.Pitch*(-1)*0.5f + ActorLocation.X;
	float YComponent = HMDRotation.Roll*0.5f + ActorLocation.Y;

	// For Z component, use the HMDPosition's Z directly, making it independent of the character's Z location
	float ZComponent = 160.f; //HMDPosition.Z;

	// Combine the components to create the HeadLocation vector
	//HeadLocation = FVector(XComponent, YComponent, ZComponent);

	// Combine the components to create the HeadLocation vector
	FVector TargetHeadLocation = FVector(XComponent, YComponent, ZComponent);

	// Smoothly interpolate the head location
	float InterpSpeed = 1000.f; // Adjust this value to control the interpolation speed
	HeadLocation = FMath::VInterpTo(HeadLocation, TargetHeadLocation, DeltaTime, InterpSpeed);



}

void AVRUnitBase::CalculateHandLocation(float DeltaTime)
{
	float InterpSpeed = 1000.f;
	if(MovementDirection.Length() > 0) InterpSpeed = 100.f;
	FVector ALocation = GetActorLocation();

	FRotator CurrentHMDRotation = HMDRotation;
	// Only use the yaw rotation for movement
	CurrentHMDRotation.Pitch = 0.0f;
	CurrentHMDRotation.Roll = 0.0f;

	// Get the forward and right vectors based on the control rotation
	FVector ForwardDirection = FRotationMatrix(CurrentHMDRotation).GetScaledAxis(EAxis::X);

	// Normalize the movement direction to ensure consistent movement speed
	ForwardDirection.Normalize();
	
	if (LeftMotionController)
	{
		FVector CurrentLeftHandPosition = LeftMotionController->GetComponentLocation();
		FVector VHeadToLHand = CurrentLeftHandPosition-HMDPosition;
		LeftHandLocation = Camera->GetComponentLocation() + VHeadToLHand - FVector(ALocation.X, ALocation.Y, 0.f) + ForwardDirection*25.f;// Camera->GetComponentLocation()+
		DrawDebugSphere(GetWorld(), LeftHandLocation, 5.0f, 12, FColor::Red, false, -1.0f, 0, 1.0f);

	}


	if (RightMotionController)
	{
		FVector CurrentRightHandPosition = RightMotionController->GetComponentLocation();
		FVector VHeadToRHand = CurrentRightHandPosition-HMDPosition;
		RightHandLocation = Camera->GetComponentLocation() + VHeadToRHand - FVector(ALocation.X, ALocation.Y, 0.f) + ForwardDirection*25.f; // Camera->GetComponentLocation()+
		//RightHandLocation = FMath::VInterpTo(RightHandLocation, CurrentRightHandPosition, DeltaTime, InterpSpeed);
		DrawDebugSphere(GetWorld(), RightHandLocation, 5.0f, 12, FColor::Blue, false, -1.0f, 0, 1.0f);
	}

	// Log the forward vector angle to the screen
	if (GEngine && MovementDirection.Length() > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("LeftHandLocation        ___: %s"), *LeftHandLocation.ToString()));
	}
	
}




void AVRUnitBase::MoveJoystick( float X, float Y)
{
	// Ensure that there is a valid movement component
	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		// Get the character's control rotation
		//FRotator ActorRotation = GetActorRotation();
		FRotator CurrentHMDRotation = HMDRotation;
		// Only use the yaw rotation for movement
		CurrentHMDRotation.Pitch = 0.0f;
		CurrentHMDRotation.Roll = 0.0f;

		// Get the forward and right vectors based on the control rotation
		FVector ForwardDirection = FRotationMatrix(CurrentHMDRotation).GetScaledAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CurrentHMDRotation).GetScaledAxis(EAxis::Y);

		// Calculate the movement direction based on the joystick input
		MovementDirection = (ForwardDirection * Y) + (RightDirection * X);

		// Normalize the movement direction to ensure consistent movement speed
		MovementDirection.Normalize();

		// Apply the movement input to the character
		AddMovementInput(MovementDirection, 0.3f);
		//CalculateHandLocation(GetWorld()->GetDeltaSeconds());
	}
}

void AVRUnitBase::SwitchMovementHandPosition()
{
	if (CurrentMovementHandPosition == MovementHandOnePosition)
	{
		CurrentMovementHandPosition = MovementHandTwoPosition;
	}
	else
	{
		CurrentMovementHandPosition = MovementHandOnePosition;
	}
}

void AVRUnitBase::SetMovementDirection( FVector Value)
{
	MovementDirection = Value;
}

void AVRUnitBase::NormalizeHMDZPosition()
{
	if (StandingZ != KneelingZ)  // To avoid division by zero
	{
		NormalizedZ = (StandingZ - HMDPosition.Z) / (StandingZ - KneelingZ);
		NormalizedZ = FMath::Clamp(NormalizedZ, 0.f, 1.f);   // Ensure the value is between 0 and 1
	}
}

void AVRUnitBase::NullActorWithHMDLocation()
{
	LastHMDActorPosition = HMDPosition;
}


void AVRUnitBase::SetActorToHMDChange(float DeltaTime)
{
	const float Threshold = 1.0f;

	FVector RelativeChange = (HMDPosition - LastHMDActorPosition);
	

	float XYMagnitude = FVector(RelativeChange.X, RelativeChange.Y, 0.0f).Size();

	if (XYMagnitude > Threshold)
	{
		SetActorLocation(GetActorLocation()+FVector(RelativeChange.X, RelativeChange.Y, 0.f));
		
		LastHMDActorPosition = HMDPosition;
	}
}

void AVRUnitBase::CalibrateHands()
{
	LeftHandLocation = GetActorLocation() + LeftHandCalibrationLocation;
	RightHandLocation = GetActorLocation() + RightHandCalibrationLocation;
	
	
	LeftHandLocationOffset = LeftMotionController->GetComponentLocation()-LeftHandLocation;
	RightHandLocationOffset = RightMotionController->GetComponentLocation()-RightHandLocation;
}

void AVRUnitBase::CalibrateMotionControllers()
{

	RightMotionController->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	LeftMotionController->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("FVector::ZeroVector ___: %s"), *FVector::ZeroVector.ToString()));
	}
	if (RightMotionController)
	{
		RightMotionController->SetRelativeLocation(FVector::ZeroVector);
		RightMotionController->SetWorldLocation(FVector::ZeroVector);
		RightMotionController->SetRelativeRotation(FRotator::ZeroRotator);
	}
    
	if (LeftMotionController)
	{
		LeftMotionController->SetRelativeLocation(FVector::ZeroVector);
		LeftMotionController->SetWorldLocation(FVector::ZeroVector);
		LeftMotionController->SetRelativeRotation(FRotator::ZeroRotator);
	}*/
}