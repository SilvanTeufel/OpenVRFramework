// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CVirt.h"
using namespace CybSDK;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Function to reset VRotationOffsetInitialised
	UFUNCTION()
	void ResetVInitialised();

	UFUNCTION()
	void StartInitTimer();
private:
	FTimerHandle TimerHandle_ResetVRotationOffsetInitialised;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// VR Root Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	class USceneComponent* VROrigin;
	
	// UCameraComponent Component for VR Headset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	bool EnableDebug = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HMDPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FRotator HMDRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HMDOffset;
	// Reference Z-Positions for normalization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float StandingZ = 180.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float KneelingZ = StandingZ/2.f;

	// Normalized Z-Position
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float CrouchedNormedZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float ScaleCapsuleIfCrouched = 0.7f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float HeadZLocation;
	
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
	FRotator LeftHandRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector MovementDirection;
	
	UFUNCTION(BlueprintCallable)
	void SetMovementDirection( FVector Value);
	
	UFUNCTION(BlueprintCallable)
	void SetRotationAndPosition();
	
	// Function to update the rotation based on HMD orientation
	UFUNCTION(BlueprintCallable)
	void UpdateRotation(FVector Position, FRotator Rotation, float Offset = 0.f);

	//UFUNCTION(BlueprintCallable)
	//void CalculateHeadLocation(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CalculateHandLocation(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CalculateHandRotation();
	
	UFUNCTION(BlueprintCallable)
	void MoveJoystick( float X, float Y, float Speed);

	UFUNCTION(BlueprintCallable)
	void MoveWithVirtualizer(float Speed, float Direction);
	
	UFUNCTION(BlueprintCallable)
	float CreateNormedVelocity(UCharacterMovementComponent* CharMovement, float Speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	float NormedVelocity;
	// Function to normalize the HMD Z-Position
	UFUNCTION(BlueprintCallable)
	void CrouchOnZPosition(float CurrentPosition);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector LastHMDActorPosition;

	// Function to update the VR Origin location every tick
	UFUNCTION(BlueprintCallable, Category = VR)
	void SetActorToHMDChange(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	bool MoveActorToHMDChange = false;

	UPROPERTY(BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FVector HMDChangeTargetLocation;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FName LeftHandSocketName = "hand_lSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR, meta = (AllowPrivateAccess = "true"))
	FName RightHandSocketName = "hand_rSocket";

	
	// FOV Settings
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	float MinFOV = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	float MaxFOV = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	float MaxSpeedForMinFOV = 300.0f; // Maximum speed at which FOV is at its minimum

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	float LastIncreaseApertureTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	float LastDecreaseApertureTime = 0.0f;
	
	UFUNCTION(BlueprintCallable , Category = VR)
	void IncreaseAperture(float Amount = 1.f, float Max = 15.f, float BlendWeight = 0.25f, float Interval = 0.1f);

	UFUNCTION(BlueprintCallable , Category = VR)
	void DecreaseAperture(float Amount = 1.f, float Min = 0.f, float BlendWeight = 0.25f, float Interval = 0.1f);
	
	UFUNCTION(BlueprintCallable , Category = Virtualizer)
	void GetVirtualizerData();

	UFUNCTION(BlueprintCallable , Category = Virtualizer)
	void HandleHapticData(int32 selection, int32 value);
	
	VirtDevice* VDevice;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Virtualizer)
	FRotator VRotation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VMovementDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VCrouch = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	bool VInitialised = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VRotationOffset = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	bool EnableVirtualizer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float MaxVSpeed = 4.f; // m/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VCrouchOffset = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VCrouchPosition = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Virtualizer)
	float VCrouchMultiplier = 1.3f;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Virtualizer)
	void VirtualizerMovement();
};
