// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/CybSDK/Virt.h"
#if PLATFORM_WINDOWS
#include "ThirdParty/CybSDK/CVirt.h"
#elif PLATFORM_ANDROID
#include "Android/Virt_Android.h"
#endif
#include "OpenVRFramework.h"
#include "Cyberith/Input/VirtInputDevice.h"

namespace CybSDK
{
	#if PLATFORM_WINDOWS
	typedef Virt NativeVirt;
	#elif PLATFORM_ANDROID
	typedef Virt_Android NativeVirt;
	#endif
}

using namespace CybSDK;


int UVirt::GetSDKVersion()
{
	return NativeVirt::GetSDKVersion();
}


UVirtDevice* UVirt::GetPluginManagedDevice()
{
	TSharedPtr<FVirtInputDevice> virtInputDevice = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

	if (virtInputDevice.IsValid() == true)
	{
		UVirtDevice* virtDevice = virtInputDevice->GetDevice().Get();
		if (virtDevice == nullptr) 
		{
			virtInputDevice->ReloadUsingSettings();
			return virtInputDevice->GetDevice().Get();
		}
		else 
		{
			return virtDevice;
		}
	}
	else
	{
		return nullptr;
	}
}

TScriptInterface<IStandaloneVirtDeviceInterface> UVirt::GetPluginManagedStandaloneInterface()
{
	TSharedPtr<FVirtInputDevice> virtInputDevice = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

	if (virtInputDevice.IsValid() == true)
	{
		TScriptInterface<IStandaloneVirtDeviceInterface> deviceInterface = virtInputDevice->GetDevice().Get()->GetStandaloneVirtDeviceInterface();
		if (deviceInterface == nullptr) 
		{
			virtInputDevice->ReloadUsingSettings();
			return virtInputDevice->GetDevice().Get()->GetStandaloneVirtDeviceInterface();
		}
		return deviceInterface;
	}
	return nullptr;
}

void UVirt::ReleaseDevice()
{
	TSharedPtr<FVirtInputDevice> virtInputDevice = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

	if (virtInputDevice.IsValid() == true)
		return virtInputDevice->ReleaseVirtDevice();
}

UVirtDevice* UVirt::FindDevice()
{
	TSharedPtr<FVirtInputDevice> virtInputDevice = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

	// Clear device in case,
	if (virtInputDevice.IsValid() == true)
		virtInputDevice->ReleaseVirtDevice();

	// Return found device
	return UVirtDevice::WrapVirtDevice(NativeVirt::FindDevice());
}

//UVirtDevice* UVirt::GetDevice(const FVirtDeviceInfo& device)
//{
//	return UVirtDevice::WrapVirtDevice(Virt::GetDevice(device.GetNativeDeviceInfo()));
//}
//
//TArray<FVirtDeviceInfo> UVirt::FindDevices()
//{
//	unsigned int numDevices;
//	const VirtDeviceInfo* deviceInfos = Virt::FindDevices(numDevices);
//
//	TArray<FVirtDeviceInfo> fDeviceInfos;
//	fDeviceInfos.Reserve(numDevices);
//
//	for (unsigned int i = 0; i < numDevices; ++i)
//	{
//		fDeviceInfos.Add(FVirtDeviceInfo(deviceInfos[i]));
//	}
//
//	delete[] deviceInfos;
//
//	return fDeviceInfos;
//}

#if PLATFORM_WINDOWS
UVirtDevice* UVirt::CreateDeviceMockupXInput()
{
	return UVirtDevice::WrapVirtDevice(NativeVirt::CreateDeviceMockupXInput());
}

UVirtDevice* UVirt::CreateDeviceMockupKeyboard()
{
	return UVirtDevice::WrapVirtDevice(NativeVirt::CreateDeviceMockupKeyboard());
}
#endif
/**
* @brief Returns the current device setup status, if it's supposed to have coupled/decoupled movement
* @return true if decoupled, false if coupled movement
*/
bool UVirt::IsDecoupled()
{
	TSharedPtr<FVirtInputDevice> virtInputDevice = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

	// Clear device in case,
	if (virtInputDevice.IsValid() == true)
	{
		return virtInputDevice->IsDecoupled();
	}
	else 
	{
		return false;
	}
}
