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
	FVector Head;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator HeadRotation;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Crouch;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Velocity;
};
