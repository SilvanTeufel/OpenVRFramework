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
	//SetRootComponent(GetMesh());
	PrimaryActorTick.bCanEverTick = true;

	// Create VR Root Component
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent);
	

	// Create Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);
	GetMesh()->SetupAttachment(GetCapsuleComponent());
/*
	// Initialize reference Z-Positions
	StandingZ = 180.f;  // Example value, adjust according to your setup
	KneelingZ = 90.f;   // Example value, adjust according to your setup
*/
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
	VDevice = Virt::FindDevice();
	//InitVirtualizerRotationOffset();
	
}



// Called every frame
void AVRUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnableVirtualizer) GetVirtualizerData();
	
	SetRotationAndPosition();
	
	if (!EnableVirtualizer) UpdateRotation(HMDPosition, HMDRotation, -90.f);
	else UpdateRotation(HMDPosition, VRotation, -90);
	
	if (!EnableVirtualizer) CrouchOnZPosition(HMDPosition.Z);



	CalculateHandLocation(DeltaTime);
	CalculateHandRotation();
	
	if (!EnableVirtualizer)
		SetActorToHMDChange(DeltaTime);
}


void AVRUnitBase::SetRotationAndPosition()
{
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);
	Camera->SetWorldRotation(HMDRotation);
}

void AVRUnitBase::UpdateRotation(FVector Position, FRotator Rotation, float Offset)
{
	// Draw a debug line from the HMD position
	FVector HMDDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
	FVector HMDLineEnd = Position + (HMDDirection * 100.0f); // Adjust the length as needed
	DrawDebugLine(GetWorld(), Position, HMDLineEnd, FColor::Green, false, -1, 0, 5.0f);

	// Get the forward and right vectors based on the control rotation

	HMDDirection.Normalize();
	// Only use the yaw rotation for the character
	FRotator NewRotation =  FRotator(0.0f, Rotation.Yaw+Offset, 0.0f);
	
	// Set the capsule component's rotation to match the HMD's yaw rotation
	GetMesh()->SetRelativeRotation(NewRotation);
	Camera->SetRelativeLocation(FVector(HMDDirection.X*10.f, HMDDirection.Y*10.f, 190.f));
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
		LeftHandLocation = Camera->GetComponentLocation() + VHeadToLHand - FVector(ALocation.X, ALocation.Y, 0.f) + ForwardDirection*25.f;
		if(EnableDebug) DrawDebugSphere(GetWorld(), LeftHandLocation, 5.0f, 12, FColor::Red, false, -1.0f, 0, 1.0f);

	}


	if (RightMotionController)
	{
		FVector CurrentRightHandPosition = RightMotionController->GetComponentLocation();
		FVector VHeadToRHand = CurrentRightHandPosition-HMDPosition;
		RightHandLocation = Camera->GetComponentLocation() + VHeadToRHand - FVector(ALocation.X, ALocation.Y, 0.f) + ForwardDirection*25.f;
		if(EnableDebug) DrawDebugSphere(GetWorld(), RightHandLocation, 5.0f, 12, FColor::Blue, false, -1.0f, 0, 1.0f);
	}

	// Log the forward vector angle to the screen
	if(EnableDebug)
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
		if(EnableDebug)
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
		// CurrentRightHandRotation.Roll = -1*CurrentRightHandRotation.Roll;  // Example of inverting the Yaw
		
		RightHandRotation = CurrentRightHandRotation;

		// Optional: Draw debug sphere to visualize rotation if needed
		if(EnableDebug)
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
		
		NormedVelocity = CreateNormedVelocity(CharMovement, Speed);
		
		NullActorWithHMDLocation();
	}
}

void AVRUnitBase::MoveWithVirtualizer(float Speed)
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


		// Calculate the movement direction based on the joystick input
		MovementDirection = ForwardDirection;

		// Normalize the movement direction to ensure consistent movement speed
		MovementDirection.Normalize();

		float NormSpeed; // Calculate NormSpeed is between 0 and 100 and dependent on Speed and MaxVSpeed. If Speed is MaxVSpeed NormSpeed is 1

		if (Speed >= 0.2f)
		{
			NormSpeed = FMath::Clamp(Speed / MaxVSpeed, 0.0f, 1.0f);
			VirtualizerMovement();
		}
		else
		{
			NormSpeed = 0.0f; // if MaxVSpeed is zero, set NormSpeed to zero
		}
		// Apply the movement input to the character
		AddMovementInput(MovementDirection, NormSpeed);
		
		NormedVelocity = CreateNormedVelocity(CharMovement, NormSpeed);
		
		NullActorWithHMDLocation();
	}
}

float AVRUnitBase::CreateNormedVelocity(UCharacterMovementComponent* CharMovement, float Speed)
{
	if(Speed == 0.0f) return 0.f;
	FVector CVelocity = GetVelocity();
	float CurrentSpeed = CVelocity.Size();
	
	// Get the character's max speed from the movement component
	float MaxSpeed = CharMovement->MaxWalkSpeed*Speed;

	// Normalize the speed to a 0-100 scale
	float NormalizedSpeed = (CurrentSpeed / MaxSpeed) * 100.0f;

	// Clamp the value to ensure it stays within the range 0-100
	NormalizedSpeed = FMath::Clamp(NormalizedSpeed, 0.0f, 100.0f);

	// Save the normalized speed to a variable (assuming you have a variable defined to store it)
	return NormalizedSpeed;
}

void AVRUnitBase::SetMovementDirection( FVector Value)
{
	MovementDirection = Value;
	NormedVelocity = 0;

}

void AVRUnitBase::CrouchOnZPosition(float CurrentPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentPosition: %f"), CurrentPosition);
	if(CurrentPosition > StandingZ)
		CurrentPosition = StandingZ;

	HeadZLocation = CurrentPosition;
	// HMDPosition.Z or 
	if (StandingZ != KneelingZ)  // To avoid division by zero
	{
		float ExponentialScale = FMath::Exp(-ScaleCapsuleIfCrouched);
		CrouchedNormedZ = (StandingZ - CurrentPosition) / (StandingZ - KneelingZ);
		CrouchedNormedZ = FMath::Clamp(CrouchedNormedZ, 0.f, 1.f)*100.f;   // Ensure the value is between 0 and 1
		GetCapsuleComponent()->SetCapsuleHalfHeight(88.f - CrouchedNormedZ*ExponentialScale);
	}
}


void AVRUnitBase::NullActorWithHMDLocation()
{
	LastHMDActorPosition = HMDPosition;
	MoveActorToHMDChange = false;
}



void AVRUnitBase::SetActorToHMDChange(float DeltaTime)
{
	
	const float Threshold = 10.0f;

	FVector RelativeChange = (HMDPosition - LastHMDActorPosition);
	

	float XYMagnitude = FVector(RelativeChange.X, RelativeChange.Y, 0.0f).Size();

	if (XYMagnitude > Threshold && !MoveActorToHMDChange)
	{
		MoveActorToHMDChange = true;
		//SetActorLocation(GetActorLocation()+FVector(RelativeChange.X, RelativeChange.Y, 0.f));
		HMDChangeTargetLocation = GetActorLocation()+FVector(RelativeChange.X, RelativeChange.Y, 0.f);
		LastHMDActorPosition = HMDPosition;
	}


	if(MoveActorToHMDChange)
	{
		// Calculate Movement Direction from Actor to HMDChangeTargetLocation
		// Move Character by using SetActorLocation

		// Calculate Movement Direction from Actor to HMDChangeTargetLocation
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (HMDChangeTargetLocation - CurrentLocation).GetSafeNormal();

		// Define movement speed (units per second)
		float MovementSpeed = 35.0f;

		// Calculate the distance to move this frame
		float MoveDistance = MovementSpeed * DeltaTime;

		// Calculate the remaining distance to the target
		float DistanceToTarget = FVector::Dist(CurrentLocation, HMDChangeTargetLocation);

		if (MoveDistance >= DistanceToTarget)
		{
			// Move directly to the target location and stop moving
			SetActorLocation(HMDChangeTargetLocation);
			MoveActorToHMDChange = false;
		}
		else
		{
			// Move towards the target location
			FVector NewLocation = CurrentLocation + Direction * MoveDistance;
			SetActorLocation(NewLocation);
		}
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

void AVRUnitBase::GetVirtualizerData() // 1.f / 0.f / 0.25f
{
	if(EnableDebug)
	{
		if (VDevice == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Device Found!"));
			UE_LOG(LogTemp, Warning, TEXT("========================================"));
			return;
		}

		const VirtDeviceInfo& info = VDevice->GetDeviceInfo();

		UE_LOG(LogTemp, Warning, TEXT("Device Found: %ls"), info.ProductName);
		UE_LOG(LogTemp, Warning, TEXT("Firmware Version: %d.%d"), info.MajorVersion, info.MinorVersion);
		UE_LOG(LogTemp, Warning, TEXT("========================================\n"));
	


		if (!VDevice->Open())
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to connect to Virtualizer!"));
			UE_LOG(LogTemp, Warning, TEXT("========================================"));
			return;
		}
	
		UE_LOG(LogTemp, Warning, TEXT("========================================"));
		UE_LOG(LogTemp, Warning, TEXT("Ring Height:        %10.2fcm"), VDevice->GetPlayerHeight());
		UE_LOG(LogTemp, Warning, TEXT("Player Orientation: %10.2f°"), VDevice->GetPlayerOrientation() * 360);
		UE_LOG(LogTemp, Warning, TEXT("Movement Speed:     %10.2fm/s"), VDevice->GetMovementSpeed());
		UE_LOG(LogTemp, Warning, TEXT("Movement Direction: %10.2f°"), VDevice->GetMovementDirection() * 180);
		UE_LOG(LogTemp, Warning, TEXT("========================================"));
	}




// Retrieve current values
    float CurrentPlayerHeight = VDevice->GetPlayerHeight();
    float CurrentPlayerOrientation = VDevice->GetPlayerOrientation() * 360.0f;
    float CurrentMovementSpeed = VDevice->GetMovementSpeed();
    float CurrentMovementDirection = VDevice->GetMovementDirection() * 180.0f;

    // Store current values into arrays
    VPlayerHeightValues[VIndex] = CurrentPlayerHeight;
    VPlayerOrientationValues[VIndex] = CurrentPlayerOrientation;
    VMovementSpeedValues[VIndex] = CurrentMovementSpeed;
    VMovementDirectionValues[VIndex] = CurrentMovementDirection;

    // Update index and check if arrays are fully populated
    VIndex = (VIndex + 1) % VNumSamples;
    if (VIndex == 0)
    {
        VInitialized = true; // After the first 10 values are collected
    }

    // Determine the number of valid samples
    int NumValidSamples = VInitialized ? VNumSamples : VIndex;

    // Compute averages
    float SumPlayerHeight = 0.0f;
    float SumPlayerOrientation = 0.0f;
    float SumMovementSpeed = 0.0f;
    float SumMovementDirection = 0.0f;

    for (int i = 0; i < NumValidSamples; ++i)
    {
        SumPlayerHeight += VPlayerHeightValues[i];
        SumPlayerOrientation += VPlayerOrientationValues[i];
        SumMovementSpeed += VMovementSpeedValues[i];
        SumMovementDirection += VMovementDirectionValues[i];
    }

    float AvgPlayerHeight = SumPlayerHeight / NumValidSamples;
    float AvgPlayerOrientation = SumPlayerOrientation / NumValidSamples;
    float AvgMovementSpeed = SumMovementSpeed / NumValidSamples;
    float AvgMovementDirection = SumMovementDirection / NumValidSamples;

	if(EnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("========================================"));
		UE_LOG(LogTemp, Warning, TEXT("Average Ring Height:        %10.2f cm"), AvgPlayerHeight);
		UE_LOG(LogTemp, Warning, TEXT("Average Player Orientation: %10.2f°"), AvgPlayerOrientation);
		UE_LOG(LogTemp, Warning, TEXT("Average Movement Speed:     %10.2f m/s"), AvgMovementSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Average Movement Direction: %10.2f°"), AvgMovementDirection);
		UE_LOG(LogTemp, Warning, TEXT("========================================"));
	}
	
	MoveWithVirtualizer(VDevice->GetMovementSpeed());
	CrouchOnZPosition((AvgPlayerHeight-50.f)*1.3f+80.f); // +VCrouchOffset
	
	if(!VRotationOffsetInitialised && VInitialized)
	{
		VRotationOffset = HMDRotation.Yaw-AvgPlayerOrientation;
		VRotationOffsetInitialised = true;
	}

	VRotation =  FRotator(0.0f, (VDevice->GetPlayerOrientation() * 360.0f)+VRotationOffset, 0.0f);
}

void AVRUnitBase::HandleHapticData(int32 selection, int32 value)
{
	if(EnableDebug)
    if (VDevice == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Device Found!"));
        return;
    }

	if(EnableDebug)
    if (!VDevice->Open())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to connect to Virtualizer!"));
        return;
    }

	if(EnableDebug)
	{
		// Log available options
		UE_LOG(LogTemp, Warning, TEXT("Choose function:"));
		UE_LOG(LogTemp, Warning, TEXT("0: Toggle active"));
		UE_LOG(LogTemp, Warning, TEXT("1: Set Gain"));
		UE_LOG(LogTemp, Warning, TEXT("2: Set Frequency"));
		UE_LOG(LogTemp, Warning, TEXT("3: Set Volume"));
		UE_LOG(LogTemp, Warning, TEXT("4: Quit"));
		UE_LOG(LogTemp, Warning, TEXT("selection: %d"), selection);
	}

        switch (selection)
        {
            case 0:
                // Toggle the haptic device's state
                VDevice->HapticStop();
        		if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic feedback stopped."));
                break;
	        case 1:
        		// Set gain
        		VDevice->HapticPlay();
        		if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic feedback started."));
        		break;
            case 2:
                // Set gain
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Set Gain (0-3):"));
        	
                VDevice->HapticSetGain(selection);
        	
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic Gain set to: %d"), value);
                break;

            case 3:
                // Set frequency
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Set Frequency (10-80):"));
        	
                VDevice->HapticSetFrequency(selection);
        	
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic Frequency set to: %d"), value);
                break;

            case 4:
                // Set volume
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Set Volume (0-100):"));
        	
                VDevice->HapticSetVolume(selection);
        	
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic Volume set to: %d"), value);
                break;

            default:
                // Stop haptic feedback and exit
                VDevice->HapticStop();
                if(EnableDebug) UE_LOG(LogTemp, Warning, TEXT("Haptic feedback stopped. Exiting."));
                break;
        }
}