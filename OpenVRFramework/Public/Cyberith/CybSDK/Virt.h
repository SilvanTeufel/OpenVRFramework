// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VirtDevice.h"
#include "VirtDeviceInfo.h"
#include "Cyberith/Standalone/StandaloneVirtDeviceInterface.h"
#include "Virt.generated.h"

/**
 *  
 */
UCLASS()
class OPENVRFRAMEWORK_API UVirt : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * @brief Returns the version number of the Virtualizer SDK.
	 * @returns (MajorVersion << 8) + MinorVersion.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static int GetSDKVersion();


	//****************************************************************************************
	//* Factories
	//****************************************************************************************


	/**
	 * @brief Gets the Virtualizer device object currently managed by the CybSDK Plugin.
	 *
	 * Returns FCybSDK_PluginModule::GetVirtualizerInputDevice()->GetDevice().
	 * @return A valid VirtDevice, or nullptr if no Virtualizer device is configured.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static UVirtDevice* GetPluginManagedDevice();

	/**
	 * @brief Gets the interface to the Standalone functionality of the Native Virtualizer device object currently managed by the CybSDK Plugin.
	 * 
	 * Returns TScriptInterface<IStandaloneVirtDeviceInterface>.
	 * @remark Not supported on Windows.
	 * @return A valid StandaloneVirtDeviceInterface, or nullptr if no Virtualizer device is configured or the Standalone interface is not supported.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
		static TScriptInterface<IStandaloneVirtDeviceInterface> GetPluginManagedStandaloneInterface();

	/**
	 * @brief Finds a standard Virtualizer device object.
	 *
	 * Factory methods for a Virtualizer device object.
	 * @return A valid VirtDevice, or nullptr if no Virtualizer device was found.
	 */
	 UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static void ReleaseDevice();

	/**
	 * @brief Finds a standard Virtualizer device object.
	 *
	 * Factory methods for a Virtualizer device object.
	 * @return A valid VirtDevice, or nullptr if no Virtualizer device was found.
	 */
	//UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static UVirtDevice* FindDevice();

	///**
	// * @brief Returns a standard Virtualizer device described by the given device info.
	// * @param device A device info struct returned by Virt::FindDevices.
	// * @return A valid VirtDevice, or nullptr if the Virtualizer device was not found.
	// */
	//UFUNCTION(BlueprintCallable, Category = "Virtualizer")
	//	static UVirtDevice* GetDevice(const FVirtDeviceInfo& device);

	///**
	// * @brief Finds all Virtualizer devices and returns their info.
	// * @return A VirtDeviceInfo array, or nullptr if no Virtualizer device was found.
	// */
	//UFUNCTION(BlueprintCallable, Category = "Virtualizer")
	//	static TArray<FVirtDeviceInfo> FindDevices();

	/**
	 * @brief Creates a virtualizer mockup for xInput controller.
	 * @return A virtual VirtDevice, driven by DirectX xInput.
	 */
	//UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static UVirtDevice* CreateDeviceMockupXInput();

	/**
	 * @brief Creates a virtualizer mockup for Keyboard and Mouse.
	 * @return A virtual VirtDevice, driven by Keyboard and Mouse input.
	 */
	//UFUNCTION(BlueprintCallable, Category = "Virtualizer")
		static UVirtDevice* CreateDeviceMockupKeyboard();

	/**
	* @brief Returns the current device setup status, if it's supposed to have coupled/decoupled movement
	* @return true if decoupled, false if coupled movement
	*/
	UFUNCTION(BlueprintPure, Category = "Virtualizer")
		static bool IsDecoupled();
};