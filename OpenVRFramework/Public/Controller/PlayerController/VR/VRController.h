// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EOS/EOS_PlayerController.h"
#include "VRController.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AVRController : public AEOS_PlayerController
{
	GENERATED_BODY()
	
public:
	AVRController();

protected:
	virtual void BeginPlay() override;

private:
	// Timer handle for line trace
	FTimerHandle LineTraceTimerHandle;

	// Interval for line tracing
	float LineTraceInterval = 0.15;

	// Callback function for the timer
	void LineTraceTimerCallback();
	
	
public:

	// The currently highlighted actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* TargetActor;

	// The previously highlighted actor (to remove highlight)
	UPROPERTY()
	AActor* PreviousTargetActor;


	UPROPERTY()
	class USpotLightComponent* HighlightLight;
	
	// Perform a line trace from the VR camera forward vector
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PerformLineTrace();

	// Outline a given actor
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HighlightActor(AActor* ActorToHighlight, FLinearColor OutlineColor = FLinearColor::Red);

	// Remove outline from a given actor
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RemoveHighlightFromActor(AActor* ActorToRemove);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void GrabProjectile();
};
