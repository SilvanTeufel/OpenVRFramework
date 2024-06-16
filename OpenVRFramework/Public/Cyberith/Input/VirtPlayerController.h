// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VirtInputDevice.h"
#include "Runtime/Launch/Resources/Version.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VirtPlayerController.generated.h"

class UPawnMovementComponent;

/**
 *
 */
UCLASS(ClassGroup = (Virtualizer), Blueprintable)
class OPENVRFRAMEWORK_API AVirtPlayerController : public APlayerController
{
	GENERATED_BODY()

	/** Shared Pointer to managed Virtualizer Device Controller */
	TSharedPtr<FVirtInputDevice> m_deviceController;

	/** Base Movement Component of the Pawn */
	UPROPERTY(Category = Virtualizer, VisibleInstanceOnly, meta = (DisplayName = "PawnMovementComponent"))
		UPawnMovementComponent* m_movementComponent;

	/** Character Movement Component of the Pawn */
	UPROPERTY(Category = Virtualizer, VisibleInstanceOnly, meta = (DisplayName = "CharacterMovementComponent"))
		UCharacterMovementComponent* m_characterMovementComponent;

	/** Movement Speed Multiplier, to fine tune the players speed. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, meta = (DisplayName = "Player speed multiplier", UIMin = 0, UIMax = 10, ClampMin = 0, ClampMax = 10))
		float MovementSpeedMultiplier = 1.2f;

	/** Reference to a SceneComponent that will be rotated according to the player’s orientation in the device. If not set will search for "ForwardDirection" attached to pawn. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Player Forward Direction"))
		USceneComponent* m_forwardDirection;

	UPROPERTY(Category = Virtualizer, VisibleInstanceOnly, meta = (DisplayName = "MotionVector"))
		FVector m_motionVector;

	void Internal_Possess(APawn* aPawn);
	void Internal_UnPossess();

public:

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|PlayerController")
		FVector GetMotionVector() const;

protected:
	
#if defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && ((ENGINE_MAJOR_VERSION >= 5) || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27))
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
#elif defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 22)
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
#else
	virtual void Possess(APawn* aPawn) override;
	virtual void UnPossess() override;
#endif
};
