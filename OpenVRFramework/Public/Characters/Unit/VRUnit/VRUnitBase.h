// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VRUnitBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AVRUnitBase : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRUnitBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VR)
	UMaterialInstance* StartMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UMaterialInstanceDynamic* VignetteMaterialInstance;

	void InitializeVignetteMaterial();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to apply vignette effect
	UFUNCTION(BlueprintCallable, Category = "Vignette")
	void ApplyVignetteEffect(float VignetteStrength = 0.5f, float VignetteZoom = 1.0f);

//private:
	// VR Root Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	class USceneComponent* VROrigin;
	
	// UCameraComponent Component for VR Headset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HMDPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FRotator HMDRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HMDOffset;
	// Reference Z-Positions for normalization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float StandingZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float KneelingZ;

	// Normalized Z-Position
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float NormalizedZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HeadLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* RightMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector LeftHandCalibrationLocation = FVector(80, 0, 140);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector RightHandCalibrationLocation = FVector(-80, 0, 140);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector LeftHandLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector RightHandLocationOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector LeftHandLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector RightHandLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector MovementDirection;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector MovementHandOnePosition = FVector(20.f, 42.f, 350.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector MovementHandTwoPosition = FVector(0.f, 42.f, 300.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float MovementHandInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector CurrentMovementHandPosition = MovementHandOnePosition;

	// Timer handle for switching the movement hand position
	FTimerHandle MovementHandTimerHandle;

	// Function to switch the movement hand position
	UFUNCTION(BlueprintCallable)
	void SwitchMovementHandPosition();
	
	UFUNCTION(BlueprintCallable)
	void SetMovementDirection( FVector Value);
	
	UFUNCTION(BlueprintCallable)
	void SetRotationAndPosition();
	
	// Function to update the rotation based on HMD orientation
	UFUNCTION(BlueprintCallable)
	void UpdateRotation();

	UFUNCTION(BlueprintCallable)
	void CalculateHeadLocation(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CalculateHandLocation(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void MoveJoystick( float X, float Y);
	
	// Function to normalize the HMD Z-Position
	UFUNCTION(BlueprintCallable)
	void NormalizeHMDZPosition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector LastHMDActorPosition;

	// Function to update the VR Origin location every tick
	UFUNCTION(BlueprintCallable, Category = VR)
	void SetActorToHMDChange(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = VR)
	void NullActorWithHMDLocation();

	UFUNCTION(BlueprintCallable, Category = VR)
	void AttachActorsToHand(FName SocketName, FVector HandLocation);

	// Example Blueprint-callable function to attach actors to left and right hand sockets
	UFUNCTION(BlueprintCallable, Category = VR)
	void AttachActorsToLeftHand();
	
	UFUNCTION(BlueprintCallable, Category = VR)
	void AttachActorsToRightHand();

	UFUNCTION(BlueprintCallable, Category = VR)
	void DetachActorsFromHand(FName SocketName);
	
	UFUNCTION(BlueprintCallable, Category = VR)
	void DetachActorsFromLeftHand();

	UFUNCTION(BlueprintCallable, Category = VR)
	void DetachActorsFromRightHand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FName LeftHandSocketName = "hand_lSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FName RightHandSocketName = "hand_rSocket";

	
	// FOV Settings
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	float MinFOV = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	float MaxFOV = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	float MaxSpeedForMinFOV = 300.0f; // Maximum speed at which FOV is at its minimum

	UFUNCTION(BlueprintCallable , Category = VR)
	void UpdateFOVBasedOnSpeed();

	UFUNCTION(BlueprintCallable , Category = VR)
	void SetBlende(float Value);

	UFUNCTION(BlueprintCallable , Category = VR)
	void AdjustAperture(float NewAperture);

	UFUNCTION(BlueprintCallable , Category = VR)
	void IncreaseAperture();

	UFUNCTION(BlueprintCallable , Category = VR)
	void DecreaseAperture();
};
