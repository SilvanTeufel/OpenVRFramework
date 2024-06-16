// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "Runtime/CoreUObject/Public/UObject/Package.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EVirtDeviceType : uint8
{
	/** CybSDK_Plugin will automatically choose the best available device for you. */
	Automatic 			UMETA(DisplayName = "[Automatic]"),
	/** A native Virtualizer Device hardware device. */
	NativeVirtualizer	UMETA(DisplayName = "Virtualizer Device"),
	/** A virtual IVirtDevice, driven by DirectX xInput. */
	XInput				UMETA(DisplayName = "Coupled/XInput (Xbox Controller)"),
	/** A virtual IVirtDevice, driven by Keyboard input. */
	Keyboard			UMETA(DisplayName = "Coupled/Keyboard"),
	/** If you want to use your own custom IVirtDevice implementation. */
	Custom				UMETA(DisplayName = "Custom/Added in UVirtDeviceComponent::InitVirtualizer")
};

inline FString GetEnumAsString(const TCHAR* Enum, int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, Enum, true); // ANY_PACKAGE
	if (!EnumPtr)
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

	return EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString();
}

inline FString GetEnumAsString(EVirtDeviceType EnumValue)
{
	return GetEnumAsString(TEXT("EVirtDeviceType"), static_cast<int32>(EnumValue));
}