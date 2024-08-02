// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//private:
	// VR Root Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	class USceneComponent* VROrigin;
	
	// UCameraComponent Component for VR Headset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector HMDPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FRotator HMDRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector HMDOffset;
	// Reference Z-Positions for normalization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float StandingZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float KneelingZ;

	// Normalized Z-Position
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float NormalizedZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector HeadLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* RightMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector LeftHandCalibrationLocation = FVector(80, 0, 140);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector RightHandCalibrationLocation = FVector(-80, 0, 140);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector LeftHandLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector RightHandLocationOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector LeftHandLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector RightHandLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector MovementDirection;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector MovementHandOnePosition = FVector(20.f, 42.f, 350.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector MovementHandTwoPosition = FVector(0.f, 42.f, 300.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float MovementHandInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
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

	UFUNCTION(BlueprintCallable)
	void SetVROriginLocation();
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector LastHMDActorPosition;

	// Function to update the VR Origin location every tick
	UFUNCTION(BlueprintCallable)
	void SetActorToHMDChange(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void NullActorWithHMDLocation();

	UFUNCTION(BlueprintCallable)
	void CalibrateHands();

	UFUNCTION(BlueprintCallable)
	void CalibrateMotionControllers();
};
