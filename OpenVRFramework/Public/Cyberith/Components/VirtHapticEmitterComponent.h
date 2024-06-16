// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "ComponentVisualizer.h"
#endif

#include "Runtime/Core/Public/CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VirtHapticEmitterComponent.generated.h"

class UVirtHapticListenerComponent;
class UCurveFloat;

UCLASS(ClassGroup = (Virtualizer), Blueprintable, meta = (BlueprintSpawnableComponent))
class OPENVRFRAMEWORK_API UVirtHapticEmitterComponent : public USceneComponent
{
	GENERATED_BODY()

	/** Reference to the Haptic Listener receiving Haptic Feedback. If not set will find one in scene. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Haptic Listener"))
		UVirtHapticListenerComponent* m_hapticListener = nullptr;

	/** AutoStart haptic effect when enabled. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "AutoStart Playing"))
		bool m_bAutoStartPlaying = false;

	/** Restart the feedback after it ended. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Loop"))
		bool m_bLoop = false;

	/** Duration in seconds for a feedback loop. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Timespan"))
		float m_duration = 3;

	/** Duration in seconds for a feedback loop. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Range"))
		float m_range = 400;

	/** Frequency for the haptic unit. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Frequency", ClampMin = "10.0", ClampMax = "80.0"))
		float m_frequency = 80.0;
	
	/** Animation Curve for feedback intensity over time [Normalized]. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Volume over Time"))
		UCurveFloat* m_volumeOverTime = nullptr;

	/** Animation Curve for feedback intensity distance [Normalized]. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Force over Distance"))
		UCurveFloat* m_forceOverDistance = nullptr;

	/** Is currently playing haptic feedback. */
	UPROPERTY(Category = Virtualizer, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Is Playing Haptic Feedback"))
		bool m_bIsPlaying = false;

	/** Keep Haptic Emitter active. Can be used to keep emitter active ignoring the emitters distance value. */
	UPROPERTY(Category = Virtualizer, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Keep Haptic Active"))
		bool m_bKeepActive = false;

public:
	// Sets default values for this component's properties
	UVirtHapticEmitterComponent();

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		virtual float EvaluateForce(FVector listenerPosition);

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		float GetRange() const;

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		float GetDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		float GetFrequency() const;

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		float GetKeepActive() const;

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void SetKeepActive(bool keepActive);

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void SetAutoStartPlaying(bool autoStart);
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	float m_timeStart;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};

#if WITH_EDITOR

class OPENVRFRAMEWORK_API FVirtHapticEmitterVisualizer : public FComponentVisualizer
{
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};

#endif
