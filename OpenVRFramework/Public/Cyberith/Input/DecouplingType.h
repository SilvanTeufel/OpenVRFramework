// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDecouplingType : uint8
{
	/** CybSDK_Plugin will automatically set the decoupling type for the current device. */
	Automatic 			UMETA(DisplayName = "[Automatic]"),
	/** Force all devices to be coupled, rotating the HMD with them - Default for virtual IVirtualizer devices. */
	ForceCoupled		UMETA(DisplayName = "Force Coupled"),
	/** Force all devices to be coupled - Default for a native Virtualizer device. */
	ForceDecoupled		UMETA(DisplayName = "Force Decoupled"),
};