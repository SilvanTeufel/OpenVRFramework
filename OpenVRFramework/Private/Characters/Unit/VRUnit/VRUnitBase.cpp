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

	//PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	//PostProcessComponent->bUnbound = true;  // Effekt wird auf das gesamte Level angewendet
	//PostProcessComponent->SetupAttachment(Camera);

}

// Called when the game starts or when spawned
void AVRUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Null the actor with the HMD location
	NullActorWithHMDLocation();
	InitializeVignetteMaterial();
	//PostProcessComponent->Settings.bOverride_VignetteIntensity = true;
	//PostProcessComponent->Settings.VignetteIntensity = 0.0f;  
}

void AVRUnitBase::InitializeVignetteMaterial()
{
	// Load the material from your content directory
	VignetteMaterialInstance = UMaterialInstanceDynamic::Create(StartMaterial, this);
	
	if (VignetteMaterialInstance)
	{
		// Apply the material to the camera's post-process settings
		APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		if (CameraManager)
		{
			// Use AddPostProcessBlendable to apply the material instance directly
			//CameraManager->AddPostProcessBlendable(VignetteMaterialInstance, 1.0f);
			
			Camera->PostProcessSettings.AddBlendable(VignetteMaterialInstance, 1.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VignetteMaterialInstance not found!"));
	}
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

	SetActorToHMDChange(DeltaTime);
	
	//SetBlende(1.f);
	//ApplyVignetteEffect(1.95, -0.59);
}

void AVRUnitBase::ApplyVignetteEffect(float VignetteStrength, float VignetteZoom)
{
	if (VignetteMaterialInstance)
	{
		VignetteMaterialInstance->SetScalarParameterValue(FName("VignetteStrength"), VignetteStrength);
		VignetteMaterialInstance->SetScalarParameterValue(FName("VignetteZoom"), VignetteZoom);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Vignette Material Instance is not initialized!"));
	}
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
				//HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
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

void AVRUnitBase::UpdateFOVBasedOnSpeed()
{
	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		// Get the current speed
		float CurrentSpeed = CharMovement->Velocity.Size();

		// Map the current speed to an FOV value
		float TargetFOV = FMath::GetMappedRangeValueClamped(
			FVector2D(0.0f, MaxSpeedForMinFOV), // Input range (speed)
			FVector2D(MaxFOV, MinFOV), // Output range (FOV)
			CurrentSpeed
		);

		// Set the new FOV for the camera
		//Camera->SetFieldOfView(TargetFOV);
		//Camera->SetFieldOfView(1.0f);  // Setze einen extrem niedrigen FOV-Wert
		Camera->FieldOfView = FMath::Clamp(1, 0, 30);
		// Log the speed and TargetFOV to the screen
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (for update), -1 for new message
				2.0f, // Time to display
				FColor::Yellow, // Text color
				FString::Printf(TEXT("Speed: %.2f, TargetFOV: %.2f"), CurrentSpeed, TargetFOV)
			);
		}
	}
}

void AVRUnitBase::SetBlende(float Value)
{
	// Begrenze den Wert, um sicherzustellen, dass er zwischen 0 und 1 liegt
	//PostProcessComponent->Settings.VignetteIntensity = Value;
	/*
	Camera->PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	Camera->PostProcessSettings.AutoExposureMinBrightness = 1.0f;
	Camera->PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	Camera->PostProcessSettings.AutoExposureMaxBrightness = 1.0f;
	Camera->PostProcessSettings.bOverride_LensFlareIntensity = true;
	Camera->PostProcessSettings.LensFlareIntensity = 0.0f;*/
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true; 
	//Camera->PostProcessSettings.bOverride_SceneColorTint = true;
	//Camera->PostProcessSettings.SceneColorTint = FLinearColor::Red;
	
		Camera->PostProcessBlendWeight = 0.25f;

		Camera->PostProcessSettings.VignetteIntensity = 10.f;
	
}


void AVRUnitBase::IncreaseAperture()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, // Key (for update), -1 for new message
			2.0f, // Time to display
			FColor::Yellow, // Text color
			FString::Printf(TEXT("IncreaseAperture!"))
		);
	}
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true; 
	Camera->PostProcessBlendWeight = 0.25f;
	Camera->PostProcessSettings.VignetteIntensity += 1.f;
}

void AVRUnitBase::DecreaseAperture()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, // Key (for update), -1 for new message
			2.0f, // Time to display
			FColor::Yellow, // Text color
			FString::Printf(TEXT("DecreaseAperture!"))
		);
	}

	Camera->PostProcessSettings.bOverride_VignetteIntensity = true; 
	Camera->PostProcessBlendWeight = 0.25f;
	Camera->PostProcessSettings.VignetteIntensity -= 1.f;

}