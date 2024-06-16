/************************************************************************************
Filename    :   CVirt.h
Content     :   Main entry point for all Virtualizer applications.
				class Virt handles all common tasks for setting up a connection.

Copyright 2019 Cyberith GmbH
Authors		:	Lukas Pfeifhofer, Jeroen Doggen, Stefan Radlwimmer <s.radlwimmer@cyberith.com>
Support		:	support@cyberith.com
*************************************************************************************/

#ifndef CVirt_h
#define CVirt_h

#include "CVirtExport.h"

#include "CVirtDeviceInfo.h"
#include "CVirtDevice.h"

namespace CybSDK
{
	/**
	 * @brief Entry class of the Cyberith Virtualizer SDK
	 */
	class CYBSDK_EXPORT Virt
	{
	public:

		/**
		 * @brief Returns the version number of the Virtualizer SDK.
		 * @returns (MajorVersion << 8) + MinorVersion.
		 */
		static uint16_t GetSDKVersion();


		//****************************************************************************************
		//* Factories
		//****************************************************************************************

		/**
		 * @brief Finds a standard Virtualizer device object.
		 *
		 * Factory methods for a Virtualizer device object.
		 * @return A valid VirtDevice, or nullptr if no Virtualizer device was found.
		 * @remarks To be freed using C++ delete by caller.
		 */
		static VirtDevice* FindDevice();

		/**
		 * @brief Returns a standard Virtualizer device described by the given device info.
		 * @param device A device info struct returned by Virt::FindDevices.
		 * @return A valid VirtDevice, or nullptr if the Virtualizer device was not found.
		 * @remarks To be freed using C++ delete by caller.
		 */
		static VirtDevice* GetDevice(const VirtDeviceInfo& device);

		/**
		 * @brief Finds all Virtualizer devices and returns their info.
		 * @param numDevices Number of devices found.
		 * @return A VirtDeviceInfo array, or nullptr if no Virtualizer device was found.
		 * @remarks To be freed using C++ delete[] by caller.
		 */
		static const VirtDeviceInfo* FindDevices(unsigned& numDevices);

		/**
		 * @brief Creates a virtualizer mockup for xInput controller.
		 * @return A virtual VirtDevice, driven by DirectX xInput.
		 * @remarks To be freed using C++ delete by caller.
		 */
		static VirtDevice* CreateDeviceMockupXInput();

		/**
		 * @brief Creates a virtualizer mockup for Keyboard.
		 * @return A virtual VirtDevice, driven by Keyboard input.
		 * @remarks To be freed using C++ delete by caller.
		 */
		static VirtDevice* CreateDeviceMockupKeyboard();
	};
}

#endif