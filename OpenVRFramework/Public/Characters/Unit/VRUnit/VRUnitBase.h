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

private:
	// VR Root Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* VRRoot;

	// Motion Controller Component for VR Headset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FRotator HMDOrientation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	FVector HMDPosition;

	// Reference Z-Positions for normalization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float StandingZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float KneelingZ;

	// Normalized Z-Position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR", meta = (AllowPrivateAccess = "true"))
	float NormalizedZ;

	// Function to update the rotation based on HMD orientation
	UFUNCTION(BlueprintCallable)
	void UpdateRotation();

	// Function to normalize the HMD Z-Position
	UFUNCTION(BlueprintCallable)
	void NormalizeHMDZPosition();
};
