// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EVirtDeviceHapticSource : uint8
{
	/** Deactivate the haptic unit. */
	None 			UMETA(DisplayName = "No haptic effects"),
	/** The haptic unit will be fed by VirtHapticEmitters and a VirtHapticListener */
	VirtHaptic		UMETA(DisplayName = "Virtualizer Haptic System (Emitters & Listeners)"),
	/** The haptic unit will be fed by UE4 ForceFeedbackComponents */
	ForceFeedback	UMETA(DisplayName = "UE4 Force Feedback System"),
};