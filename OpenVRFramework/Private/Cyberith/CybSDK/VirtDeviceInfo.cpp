// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/CybSDK/VirtDeviceInfo.h"
//#if PLATFORM_WINDOWS
#include "ThirdParty/CybSDK/CVirtDeviceInfo.h"
//#endif

using namespace CybSDK;

FVirtDeviceInfo::FVirtDeviceInfo()
	: MajorVersion(0), MinorVersion(0)
	, VendorId(0), VendorName("")
	, ProductId(0), ProductName("")
	, m_devicePath(nullptr)
{}

FVirtDeviceInfo::FVirtDeviceInfo(const NativeVirtDeviceInfo& nativeDeviceInfo)
	: MajorVersion(nativeDeviceInfo.MajorVersion), MinorVersion(nativeDeviceInfo.MinorVersion)
	, VendorId(nativeDeviceInfo.VendorId), VendorName(nativeDeviceInfo.VendorName)
	, ProductId(nativeDeviceInfo.ProductId), ProductName(nativeDeviceInfo.ProductName)
	, m_devicePath(nullptr)
{
	const char* devicePath = nativeDeviceInfo.GetDevicePath();
	if (devicePath == nullptr)
		return;

	int32 SrcLen = TCString<char>::Strlen(devicePath) + 1;
	m_devicePath = new char[SrcLen];
	TCString<char>::Strcpy(m_devicePath, SrcLen, devicePath);
}

NativeVirtDeviceInfo FVirtDeviceInfo::GetNativeDeviceInfo() const
{
	checkf(MajorVersion == 0, TEXT("UVirtDevice not initialized!"));
	return NativeVirtDeviceInfo(MajorVersion, MinorVersion, VendorId, *VendorName, ProductId, *ProductName, m_devicePath);
}
