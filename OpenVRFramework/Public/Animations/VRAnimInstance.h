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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector LeftHandPosition;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RightHandPosition;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator LeftHandRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator SmoothedLeftHandRot;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator RightHandRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator SmoothedRightHandRot;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FTransform RightHandTargetTM;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FTransform LeftHandTargetTM;
	// tweakable interp speed (higher = snappier; lower = softer)
	UPROPERTY(EditAnywhere, Category="VR|Smoothing")
	float RotationInterpSpeed = 12.f;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float CalculatedPitchRotation;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float HeadZLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator HeadRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector HeadLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector WorldHeadLocation;
	
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

};
