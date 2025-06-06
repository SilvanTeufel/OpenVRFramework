// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile.h"
#include "Components/PointLightComponent.h"
#include "EOS/EOS_PlayerController.h"
#include "Components/BillboardComponent.h"
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

	virtual void Tick(float DeltaSeconds) override;
	
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

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AProjectile* GrabbedProjectile;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	FVector GrabbedProjectileHandSensitivity = FVector(3.f,3.f,4.f);
	
	UPROPERTY()
	class USpotLightComponent* HighlightLight;
	
	// Perform a line trace from the VR camera forward vector
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PerformLineTrace();

	UPROPERTY()
	UPointLightComponent* CrosshairLight;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void AddOrUpdateCrosshairLight(const FVector& Location, bool bShouldBeVisible);

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	UBillboardComponent* CrosshairBillboard;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void AddOrUpdateCrosshair(const FVector& Location, bool bShouldBeVisible);
	// Outline a given actor
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HighlightActor(AActor* ActorToHighlight, FLinearColor OutlineColor = FLinearColor::Red);

	// Remove outline from a given actor
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RemoveHighlightFromActor(AActor* ActorToRemove);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void GrabProjectile( AVRUnitBase* VRUnit);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HandleGrabbedProjectile();
	
};
