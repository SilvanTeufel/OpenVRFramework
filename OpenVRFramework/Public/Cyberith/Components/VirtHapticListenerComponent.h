// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cyberith/Input/VirtInputDevice.h"
#include "Cyberith/CybSDK/VirtDevice.h"
#include "VirtHapticListenerComponent.generated.h"

class UVirtHapticEmitterComponent;

UCLASS(ClassGroup = (Virtualizer), Blueprintable, meta = (BlueprintSpawnableComponent))
class OPENVRFRAMEWORK_API UVirtHapticListenerComponent : public UActorComponent
{
	GENERATED_BODY()

		TSharedPtr<FVirtInputDevice> m_deviceController;

	UPROPERTY(Category = Virtualizer, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Registered Haptic Emitters"))
		TArray<UVirtHapticEmitterComponent*> m_emitters;

	UPROPERTY(Category = Virtualizer, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Maximum Range"))
		float m_maxRange = 60 * 100; // 60 meters

public:
	// Sets default values for this component's properties
	UVirtHapticListenerComponent();

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void AddEmitter(UVirtHapticEmitterComponent* emitter);

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void RemoveEmitter(UVirtHapticEmitterComponent* emitter);

protected:

	virtual void InitializeComponent() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnVirtDeviceChanged(UVirtDevice* virtDevice, EVirtDeviceChangedType callbackType);

	static float SumUpDecibel(float a, float b);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
		override;


};
