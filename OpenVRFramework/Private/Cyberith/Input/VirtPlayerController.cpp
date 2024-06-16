// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/Input/VirtPlayerController.h"
#include "GameFramework/Pawn.h"
#include "OpenVRFramework.h"
#include "Components/SkeletalMeshComponent.h"
#include "Cyberith/CybSDK/VirtDevice.h"
#include "GameFramework/Character.h"

void AVirtPlayerController::Internal_Possess(APawn* aPawn)
{
	if (m_forwardDirection == nullptr)
	{
		USceneComponent* forwardDirectionComponent = NewObject<USceneComponent>(aPawn, USceneComponent::StaticClass(), FName("ForwardDirection"));
		forwardDirectionComponent->RegisterComponent();
		forwardDirectionComponent->AttachToComponent(aPawn->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

		// Change Name
		forwardDirectionComponent->Rename(TEXT("ForwardDirection"));

		// Fix Pos/Rot
		forwardDirectionComponent->SetRelativeLocation(FVector::ZeroVector);
		forwardDirectionComponent->SetRelativeRotation(FQuat::Identity);

		// Show in actor's details
		aPawn->AddInstanceComponent(forwardDirectionComponent);
	}

	// Search for Forward Direction
	if (m_forwardDirection == nullptr)
	{
		TArray<USceneComponent*> sceneComponents;
		aPawn->GetComponents <USceneComponent>(sceneComponents);
		for (USceneComponent* component : sceneComponents)
		{
			if (component->GetName() == TEXT("ForwardDirection"))
			{
				m_forwardDirection = component;
				break;
			}
		}
	}

	// Pawn - Movement Component
	m_movementComponent = aPawn->GetMovementComponent();

	// Get character
	ACharacter* character = GetCharacter();
	if (character != nullptr)
	{
		// Find attached character movement component
		m_characterMovementComponent = character->GetCharacterMovement();
	}

	//// Log
	//UE_LOG(LogTemp, Display, TEXT("[CYBERITH] AVirtPlayerController : Internal_Possess()"));

#if WITH_EDITOR
	// When starting the Editor we don't want UE4 to open and exclusively lock the Virtualizer -->
	// we have to manually tell the DeviceController to open a connection.
	if (m_deviceController.IsValid())
		m_deviceController->StartInEditor();
#endif
}

void AVirtPlayerController::Internal_UnPossess()
{
	m_forwardDirection = nullptr;
	m_movementComponent = nullptr;
	m_characterMovementComponent = nullptr;

	// Log
	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] AVirtPlayerController : Internal_UnPossess()"));

#if WITH_EDITOR
	// When starting the Editor we don't want UE4 to open and exclusively lock the Virtualizer -->
	// we have to manually tell the DeviceController to close the connection.
	if (m_deviceController.IsValid())
		m_deviceController->StopInEditor();
#endif
}

void AVirtPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_deviceController = FOpenVRFrameworkModule::GetVirtualizerInputDevice();
}

void AVirtPlayerController::Tick(float DeltaSeconds)
{
	
	Super::Tick(DeltaSeconds);


	/** Check components */


	// Ignore Virtualizer input if not local player
	if (!IsLocalPlayerController())
		return;

	// Ignore Virtualizer input if not possessed
	APawn* pawn = GetPawn();
	if (pawn == nullptr)
		return;

	// FCybSDK_PluginModule::GetVirtualizerInputDevice() will be initialized by the Unreal Engine in the first player tick
	if (m_deviceController.IsValid() == false)
	{
		m_deviceController = FOpenVRFrameworkModule::GetVirtualizerInputDevice();
		if (m_deviceController.IsValid() == false)
			return;
	}
	
	TWeakObjectPtr<UVirtDevice> device = m_deviceController->GetDevice();

	if (device == nullptr || !device->IsOpen())
		return;

	/** Calculate Movement & Apply Orientation */


	// Get movement speed
	FVector movement = device->GetMovementVector() * MovementSpeedMultiplier;

	// Get player orientation
	FQuat localOrientation = device->GetPlayerOrientationQuaternion();

	// Determine global orientation for characterController Movement
	FQuat globalOrientation;

	// For decoupled movement we do not rotate the pawn --> HMD does that
	if (m_deviceController->IsDecoupled())
	{
		if (m_forwardDirection != nullptr)
		{
			m_forwardDirection->SetRelativeRotation(localOrientation);
			globalOrientation = m_forwardDirection->GetComponentQuat();
		}
		else
		{
			// Quaternions are applied right to left
			globalOrientation = localOrientation * pawn->GetActorQuat();
		}
	}
	// For coupled movement we rotate the pawn and HMD
	else
	{
		pawn->SetActorRotation(localOrientation);
		globalOrientation = localOrientation;
	}

	FVector motionVector = globalOrientation * movement;
	m_motionVector = motionVector;
	
	if (motionVector.IsZero())
		return;


	/** Apply Movement */


	// If pawn is a character we use CharacterMovementComponent 
	if (m_characterMovementComponent != nullptr)
	{
		// * 100 for correct Unreal Units --> MoveSmooth(m/s * 100) --> MoveSmooth(cm)
		m_characterMovementComponent->MoveSmooth(motionVector * 100.0f, DeltaSeconds);
	}
	// If pawn has explicit MovementComponent use it
	else if (m_movementComponent != nullptr)
	{
		// AddMovementInput wants normalized vectors --> 10 is the theoretical max speed a user can achieve
		m_movementComponent->AddInputVector(motionVector / 10.0f);
	}
	// Otherwise use simple move
	else
	{
		// * 100 for correct Unreal Units --> Set( cm + m/s * 100 * s) --> Set(cm)
		pawn->SetActorLocation(pawn->GetActorLocation() + motionVector * 100.0f * DeltaSeconds);
	}
}

FVector AVirtPlayerController::GetMotionVector() const
{
	return m_motionVector;
}

#if defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && ((ENGINE_MAJOR_VERSION >= 5) || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27))
// Unreal Engine 5 preview is actually UE 4.27.2
void AVirtPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	Internal_Possess(aPawn);
}

void AVirtPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	Internal_UnPossess();
}
#elif defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 22)
void AVirtPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	Internal_Possess(aPawn);
}

void AVirtPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	Internal_UnPossess();
}


#else
void AVirtPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	Internal_Possess(aPawn);
}

void AVirtPlayerController::UnPossess()
{
	Super::UnPossess();

	Internal_UnPossess();
}
#endif