// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "Components/CapsuleComponent.h" // Add this include
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"


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
	
	// Null the actor with the HMD location
	NullActorWithHMDLocation();
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
	CalculateHandRotation();
	
	SetActorToHMDChange(DeltaTime);
	

}


void AVRUnitBase::SetRotationAndPosition()
{
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);
	Camera->SetWorldRotation(HMDRotation);
}

void AVRUnitBase::UpdateRotation()
{

	// Draw a debug line from the HMD position
	FVector HMDDirection = FRotationMatrix(HMDRotation).GetScaledAxis(EAxis::X);
	FVector HMDLineEnd = HMDPosition + (HMDDirection * 100.0f); // Adjust the length as needed
	DrawDebugLine(GetWorld(), HMDPosition, HMDLineEnd, FColor::Green, false, -1, 0, 5.0f);

	// Get the forward and right vectors based on the control rotation

	HMDDirection.Normalize();
	// Only use the yaw rotation for the character
	FRotator NewRotation =  FRotator(0.0f, HMDRotation.Yaw-90.f, 0.0f);
	
	// Set the capsule component's rotation to match the HMD's yaw rotation
	GetMesh()->SetRelativeRotation(NewRotation);
	Camera->SetRelativeLocation(FVector(HMDDirection.X*10.f, HMDDirection.Y*10.f, 190.f));
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

void AVRUnitBase::CalculateHandRotation()
{
	if (LeftMotionController)
	{
		// Retrieve the current rotation of the left motion controller
		FRotator CurrentLeftHandRotation = LeftMotionController->GetComponentRotation();
		
		// Optionally invert yaw or roll to correct the mirroring
		//CurrentLeftHandRotation.Pitch = -1*CurrentLeftHandRotation.Pitch;  // Example of inverting the Yaw

		LeftHandRotation = CurrentLeftHandRotation;

		// Optional: Draw debug sphere to visualize rotation if needed
		DrawDebugCoordinateSystem(
			GetWorld(),
			LeftMotionController->GetComponentLocation(),
			LeftHandRotation,
			20.0f, // Size of the coordinate system
			false, // Persistent lines
			-1.0f, // Lifetime
			0, // Depth priority
			1.0f // Line thickness
		);
	}

	if (RightMotionController)
	{
		// Retrieve the current rotation of the right motion controller
		FRotator CurrentRightHandRotation = RightMotionController->GetComponentRotation();

		// Optionally invert yaw or roll to correct the mirroring
		//CurrentRightHandRotation.Roll = -1*CurrentRightHandRotation.Roll;  // Example of inverting the Yaw
		
		RightHandRotation = CurrentRightHandRotation;

		// Optional: Draw debug sphere to visualize rotation if needed
		DrawDebugCoordinateSystem(
			GetWorld(),
			RightMotionController->GetComponentLocation(),
			RightHandRotation,
			20.0f, // Size of the coordinate system
			false, // Persistent lines
			-1.0f, // Lifetime
			0, // Depth priority
			1.0f // Line thickness
		);
	}
}


void AVRUnitBase::MoveJoystick( float X, float Y, float Speed)
{
	// Ensure that there is a valid movement component
	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		// Get the character's control rotation
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
		AddMovementInput(MovementDirection, Speed);
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

void AVRUnitBase::AttachActorsToHand(FName SocketName, FVector HandLocation)
{
	TArray<FHitResult> OutHits;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(
		OutHits,
		HandLocation,
		HandLocation + FVector(1.0f, 0.0f, 0.0f), // small movement for detection
		FQuat::Identity,
		ECC_PhysicsBody,
		FCollisionShape::MakeSphere(10.0f),
		CollisionParams
	);

	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetRootComponent())
		{
			if (GetMesh()->DoesSocketExist(SocketName))
			{
				HitActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				HitActor->SetActorRotation(FRotator::ZeroRotator);
			}
		}
	}
}

// Example Blueprint-callable function to attach actors to left and right hand sockets

void AVRUnitBase::AttachActorsToLeftHand()
{
	AttachActorsToHand(LeftHandSocketName, LeftHandLocation);
}


void AVRUnitBase::AttachActorsToRightHand()
{
	AttachActorsToHand(RightHandSocketName, RightHandLocation);
}


void AVRUnitBase::DetachActorsFromHand(FName SocketName)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("DetachActorsFromHand called"));
	}
	// Get all components attached to the character's mesh
	TArray<USceneComponent*> AttachedComponents;
	GetMesh()->GetChildrenComponents(false, AttachedComponents);

	// Loop through each attached component
	for (USceneComponent* Component : AttachedComponents)
	{
		// Check if the component is attached to the specified socket
		if (Component->GetAttachSocketName() == SocketName)
		{
			AActor* AttachedActor = Component->GetOwner();
			if (AttachedActor && AttachedActor != this)
			{
				// Detach the actor from the mesh
				AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

// Example Blueprint-callable function to detach actors from left and right hand sockets
void AVRUnitBase::DetachActorsFromLeftHand()
{
	DetachActorsFromHand(LeftHandSocketName);
}

void AVRUnitBase::DetachActorsFromRightHand()
{
	DetachActorsFromHand(RightHandSocketName);
}

void AVRUnitBase::IncreaseAperture(float Amount, float Max, float BlendWeight, float Interval) // 1.f / 25.f / 0.25f
{
	if (GetWorld()->TimeSeconds - LastIncreaseApertureTime < Interval)
		return; // Skip execution if not enough time has passed

	LastIncreaseApertureTime = GetWorld()->TimeSeconds; // Update the last execution time

	Camera->PostProcessSettings.bOverride_VignetteIntensity = true; 
	Camera->PostProcessBlendWeight = BlendWeight;
	if(Camera->PostProcessSettings.VignetteIntensity < Max)
		Camera->PostProcessSettings.VignetteIntensity += Amount;
}

void AVRUnitBase::DecreaseAperture(float Amount, float Min, float BlendWeight, float Interval) // 1.f / 0.f / 0.25f
{
	FVector Velo = GetVelocity();
	if (!Velo.IsZero()) return;  // Only proceed if velocity is zero

	if (GetWorld()->TimeSeconds - LastDecreaseApertureTime < Interval)
		return; // Skip execution if not enough time has passed

	LastDecreaseApertureTime = GetWorld()->TimeSeconds;
	
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true; 
	Camera->PostProcessBlendWeight = BlendWeight;
	if(Camera->PostProcessSettings.VignetteIntensity > Min) Camera->PostProcessSettings.VignetteIntensity -= Amount;

}