// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "VirtDeviceType.h"
#include "DecouplingType.h"
#include "VirtDeviceHapticSource.h"
#include "VirtInputSettings.generated.h"

/**
 * Settings determining how your game connects to the Virtualizer Device. 
 */
UCLASS(config = "Input", defaultconfig, meta = (DisplayName = "Virtualizer Connection Settings"))
class OPENVRFRAMEWORK_API UVirtInputSettings : public UObject
{
	GENERATED_BODY()

public:
	UVirtInputSettings(const FObjectInitializer& ObjectInitializer);

	/** Select the type of device you want to use. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, Config, meta = (ConfigRestartRequired = false, DisplayName = "Device Type"))
		EVirtDeviceType DeviceType = EVirtDeviceType::Automatic;

	/** Override the default decoupling behaviour of the active VirtDevice. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, Config, meta = (ConfigRestartRequired = false, DisplayName = "Decoupling Override"))
		EDecouplingType DecouplingType = EDecouplingType::Automatic;

	/** Select how the Virtualizer haptic unit will be activated. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, Config, meta = (ConfigRestartRequired = false, DisplayName = "Haptics Source System"))
		EVirtDeviceHapticSource HapticSource = EVirtDeviceHapticSource::VirtHaptic;

	/** Resets height and orientation on connect - Unused due to absolute tracking. */
	UPROPERTY(Category = Virtualizer, EditAnywhere, Config, meta = (ConfigRestartRequired = false, DisplayName = "Calibrate on connect"))
		bool bCalibrateOnConnect = false;
};
