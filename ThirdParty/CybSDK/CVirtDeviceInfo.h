/************************************************************************************
Filename    :   CVirtDeviceInfo.h
Content     :   struct VirtDeviceInfo holds all information about a Virtualizer device.
				Typically returned by Virt::FindDevices(...) and VirtDevice::GetDeviceInfo().

Copyright 2019 Cyberith GmbH
Authors		:   Lukas Pfeifhofer, Jeroen Doggen, Stefan Radlwimmer <s.radlwimmer@cyberith.com>
Support		:	support@cyberith.com
*************************************************************************************/

#ifndef CVirtDeviceInfo_h
#define CVirtDeviceInfo_h

#include <cstdint>
#include "CVirtExport.h"

namespace CybSDK
{
	/**
	 * @brief Struct containing all infos about a Virtualizer USB device.
	 */
	struct CYBSDK_EXPORT VirtDeviceInfo
	{
		/**
		 * @brief The public USB vendor ID owned by Cyberith GmbH.
		 */
		static const uint16_t CyberithVID;

		/**
		 * \brief Default Constructor
		 */
		VirtDeviceInfo();
		/**
		 * \brief Constructor for a simulated Virtualizer device
		 */
		VirtDeviceInfo(uint8_t majorVersion, uint8_t minorVersion, uint16_t vendorId, const wchar_t* vendorName, uint16_t productId, const wchar_t* productName);
		/**
		 * \brief Constructor for a Virtualizer device
		 */
		VirtDeviceInfo(uint8_t majorVersion, uint8_t minorVersion, uint16_t vendorId, const wchar_t* vendorName, uint16_t productId, const wchar_t* productName, const char* devicePath);
		/**
		 * \brief Copy Constructor
		 */
		VirtDeviceInfo(const VirtDeviceInfo& other);
		/**
		 * \brief Assignment operator
		 */
		VirtDeviceInfo& operator=(const VirtDeviceInfo& other);
		/**
		 * \brief Destructor
		 */
		~VirtDeviceInfo();
		
		/**
		 * \brief Returns the name for a given vendor Id.
		 * \return The name of a known vendor, otherwise "Unknown Vendor".
		 */
		static const wchar_t* GetVendorNameFromId(uint16_t vid);
		/**
		 * \brief Returns the name for a given product Id.
		 * \return The name of a known product, otherwise "Unknown Device".
		 */
		static const wchar_t* GetProductNameFromId(uint16_t pid);
		
		/**
		 * @brief Returns the UBS device path used for the HID connection.
		 */
		const char* GetDevicePath() const;
		
		/**
		 * @brief The major firmware version.
		 */
		uint8_t MajorVersion;
		/**
		 * @brief The minor firmware version.
		 */
		uint8_t MinorVersion;

		/**
		 * @brief The USB vendor id.
		 */
		uint16_t VendorId = CyberithVID;
		/**
		 * @brief The USB vendor name.
		 */
		wchar_t* VendorName;


		/**
		 * @brief The USB product id.
		 */
		uint16_t ProductId;
		/**
		 * @brief The USB product name.
		 */
		wchar_t* ProductName;

	private:
		/**
		 * @brief The UBS device path used for the HID connection.
		 */
		char* m_devicePath;
	};

}

#endif


