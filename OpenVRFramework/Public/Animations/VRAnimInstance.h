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
	FVector LeftHand;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RightHand;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector Head;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Knee;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Velocity;
};
