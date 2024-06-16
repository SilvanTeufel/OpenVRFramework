// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/VirtDeviceInfo_Android.h"
#endif
#include "VirtDeviceInfo.generated.h"

namespace CybSDK
{
	#if PLATFORM_WINDOWS
	struct VirtDeviceInfo;
	typedef VirtDeviceInfo NativeVirtDeviceInfo;
	#elif PLATFORM_ANDROID
	typedef FVirtDeviceInfo_Android NativeVirtDeviceInfo;
	#endif
}

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct OPENVRFRAMEWORK_API FVirtDeviceInfo
{
	GENERATED_BODY()

public:

	FVirtDeviceInfo();
	explicit FVirtDeviceInfo(const CybSDK::NativeVirtDeviceInfo& nativeDeviceInfo);

	CybSDK::NativeVirtDeviceInfo GetNativeDeviceInfo() const;

	/**
	 * @brief The major firmware version.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		int32 MajorVersion;
	/**
	 * @brief The minor firmware version.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		int32 MinorVersion;

	/**
	 * @brief The USB vendor id.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		int32 VendorId;
	/**
	 * @brief The USB vendor name.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		FString VendorName;


	/**
	 * @brief The USB product id.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		int32 ProductId;
	/**
	 * @brief The USB product name.
	 */
	UPROPERTY(Category = "Virtualizer|Device|Info", VisibleInstanceOnly, BlueprintReadOnly)
		FString ProductName;

protected:
	/**
	 * @brief The UBS device path used for the HID connection.
	 */
	char* m_devicePath;
};
