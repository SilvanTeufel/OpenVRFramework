// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UVRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual void NativeUpdateAnimation(float Deltaseconds) override;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector LeftHandPosition;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RightHandPosition;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator LeftHandRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator RightHandRotation;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float HeadZLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator HeadRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector HeadLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsVirtualizerEnabled = false;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector ActorLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator ActorRotation;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float VRotationOffset;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator VRotation;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Crouch;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Velocity;
	
	
	
	// Accumulated unwrapped Yaw value
	float UnwrappedYaw = 0.0f;

	// Previous raw Yaw value
	float PreviousRawYaw = 0.0f;

	// Initial unwrapped Yaw value (for normalization reference)
	float InitialUnwrappedYaw = 0.0f;

	// Flag to check if this is the first update
	bool bFirstUpdate = true;
};
