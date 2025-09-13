// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/CybSDK/VirtDevice.h"
//#if PLATFORM_WINDOWS
#include "ThirdParty/CybSDK/CVirtDevice.h"
//#endif
#include "Cyberith/CybSDKUtils.h"


using namespace CybSDK;

UVirtDevice::UVirtDevice(): m_device(nullptr)
{}

UVirtDevice::~UVirtDevice()
{
#if PLATFORM_ANDROID
	CybSDKUtils::FreeObjectForGarbageCollection(m_device);
#endif
	delete m_device;
	m_device = nullptr;
}

void UVirtDevice::Init(NativeVirtDevice* device)
{
	checkf(m_device == nullptr, TEXT("UVirtDevice is already initialized!"));
	m_device = device;
	#if PLATFORM_ANDROID
	CybSDKUtils::SaveObjectFromGarbageCollection(m_device);
	#endif
	m_fDeviceInfo = FVirtDeviceInfo(m_device->GetDeviceInfo());
}

const NativeVirtDevice * UVirtDevice::GetNativeDevice() const
{
	checkf(m_device != nullptr, TEXT("UVirtDevice not initialized!"));
	return m_device;
}

NativeVirtDevice* UVirtDevice::GetNativeDevice()
{
	checkf(m_device != nullptr, TEXT("UVirtDevice not initialized!"));
	return m_device;
}

TScriptInterface<IStandaloneVirtDeviceInterface> UVirtDevice::GetStandaloneVirtDeviceInterface()
{
	#if PLATFORM_ANDROID
	checkf(m_device != nullptr, TEXT("UVirtDevice not initialized!"));
	TScriptInterface<IStandaloneVirtDeviceInterface> StandaloneVirtDeviceInterfaceInterface;
	StandaloneVirtDeviceInterfaceInterface.SetObject(m_device);
	StandaloneVirtDeviceInterfaceInterface.SetInterface(Cast<IStandaloneVirtDeviceInterface>(m_device));
	return StandaloneVirtDeviceInterfaceInterface;
	//return m_device;
	#else
	UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] UVirtDevice::GetStandaloneVirtDeviceInterface : Standalone Interface of the CybSDK is not supported for this system, returning nullptr"));
	return nullptr;
	#endif
}

bool UVirtDevice::Open()
{
	checkf(m_device != nullptr, TEXT("UVirtDevice not initialized!"));
	return m_device->Open();
}

bool UVirtDevice::IsOpen() const
{
	return (m_device != nullptr && m_device->IsOpen());
}

void UVirtDevice::Close()
{
	if (m_device)
	{
		m_device->Close();      // SDK teardown
		delete m_device;        // or however you free it
		m_device = nullptr;     // <— so you never get a -1 in there
	}
}

const FVirtDeviceInfo& UVirtDevice::GetDeviceInfo() const
{
	return m_fDeviceInfo;
}

float UVirtDevice::GetPlayerHeight() const
{
	if (m_device != nullptr)
		return m_device->GetPlayerHeight();
	return 0.0f;
}

void UVirtDevice::ResetPlayerHeight()
{
	if (m_device != nullptr)
		m_device->ResetPlayerHeight();
}

float UVirtDevice::GetPlayerOrientation() const
{
	if (m_device != nullptr)
		return m_device->GetPlayerOrientation();
	return 0.0f;
}

void UVirtDevice::ResetPlayerOrientation()
{
	if (m_device != nullptr)
		m_device->ResetPlayerOrientation();
}

FVector UVirtDevice::GetPlayerOrientationVector() const
{
	const float playerOrientation = GetPlayerOrientation() * 2 * PI;
	FVector dir(
		FMath::Cos(playerOrientation),
		FMath::Sin(playerOrientation),
		0.0f);
	dir.Normalize();
	return dir;
}

FQuat UVirtDevice::GetPlayerOrientationQuaternion() const
{
	const float playerOrientation = GetPlayerOrientation() * 360.0f;
	return FQuat::MakeFromEuler(FVector(0, 0, playerOrientation));
}

float UVirtDevice::GetMovementSpeed() const
{
	if (m_device != nullptr)
		return m_device->GetMovementSpeed();
	return 0.0f;
}

float UVirtDevice::GetMovementDirection() const
{
	if (m_device != nullptr)
		return m_device->GetMovementDirection();
	return 0.0f;
}

FVector UVirtDevice::GetMovementVector() const
{
	return GetMovementDirectionVector() * GetMovementSpeed();
}

FVector UVirtDevice::GetMovementDirectionVector() const
{
	const float movementDirection = GetMovementDirection() * PI;
	FVector dir(
		FMath::Cos(movementDirection),
		FMath::Sin(movementDirection),
		0.0f);
	dir.Normalize();
	return dir;
}

bool UVirtDevice::HasHaptic() const
{
	if (m_device != nullptr)
		return m_device->HasHaptic();
	return false;
}

void UVirtDevice::HapticPlay()
{
	if (m_device != nullptr)
		m_device->HapticPlay();
}

void UVirtDevice::HapticStop()
{
	if (m_device == nullptr)
		return;
	
	m_device->HapticStop();
}

void UVirtDevice::HapticSetGain(int gain)
{
	if (m_device != nullptr)
		m_device->HapticSetGain(gain);
}

void UVirtDevice::HapticSetFrequency(int frequency)
{
	if (m_device != nullptr)
		m_device->HapticSetFrequency(frequency);
}

void UVirtDevice::HapticSetVolume(int volume)
{
	if (m_device != nullptr)
		m_device->HapticSetVolume(volume);
}

UVirtDevice* UVirtDevice::WrapVirtDevice(NativeVirtDevice* device)
{
	if (device == nullptr)
		return nullptr;

	UVirtDevice* uDevice = NewObject<UVirtDevice>();
	uDevice->Init(device);
	return uDevice;
}