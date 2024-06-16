// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/Input/VirtInputDevice.h"
#include "Engine/InputAxisKeyDelegateBinding.h"
#include "Cyberith/CybSDK/Virt.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GenericPlatform/IInputInterface.h"
#include "Cyberith/CybSDKUtils.h"




#if WITH_EDITOR
#include <Editor/UnrealEd/Public/Editor.h>
#endif

#define LOCTEXT_NAMESPACE "CybSDK"

namespace CyberithKeyNames
{
	const FGamepadKeyNames::Type MoveForward("Cyberith_Virtualizer_Move_Forward");
	const FGamepadKeyNames::Type MoveRight("Cyberith_Virtualizer_Move_Right");
	const FGamepadKeyNames::Type Orientation("Cyberith_Virtualizer_Orientation");
}

FVirtInputDevice::FVirtInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: m_settings(GetMutableDefault<UVirtInputSettings>())
	, MessageHandler(InMessageHandler)
{
	//// Log
	//UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FVirtInputDevice::FVirtInputDevice()"));

	// Cyberith specific
	EKeys::AddMenuCategoryDisplayInfo("Cyberith", LOCTEXT("CyberithSubCateogry", "Cyberith"), TEXT("GraphEditor.PadEvent_16x"));

#if defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && ((ENGINE_MAJOR_VERSION >= 5) || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27))
	// Register the FKeys (Gamepad key for controllers, Mouse for mice, FloatAxis for non binary values e.t.c.)
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveForward, LOCTEXT("Cyberith_Virtualizer_Move_Forward", "Cyberith Virtualizer - Move Forward"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveRight, LOCTEXT("Cyberith_Virtualizer_Move_Right", "Cyberith Virtualizer - Move Right"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::Orientation, LOCTEXT("Cyberith_Virtualizer_Orientation", "Cyberith Virtualizer - Absolute Orientation"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
#elif defined(ENGINE_MAJOR_VERSION) && defined(ENGINE_MINOR_VERSION) && (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	// Register the FKeys (Gamepad key for controllers, Mouse for mice, FloatAxis for non binary values e.t.c.)
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveForward, LOCTEXT("Cyberith_Virtualizer_Move_Forward", "Cyberith Virtualizer - Move Forward"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveRight, LOCTEXT("Cyberith_Virtualizer_Move_Right", "Cyberith Virtualizer - Move Right"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::Orientation, LOCTEXT("Cyberith_Virtualizer_Orientation", "Cyberith Virtualizer - Absolute Orientation"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, "Cyberith"));
#else
	// Register the FKeys (Gamepad key for controllers, Mouse for mice, FloatAxis for non binary values e.t.c.)
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveForward, LOCTEXT("Cyberith_Virtualizer_Move_Forward", "Cyberith Virtualizer - Move Forward"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::MoveRight, LOCTEXT("Cyberith_Virtualizer_Move_Right", "Cyberith Virtualizer - Move Right"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, "Cyberith"));
	EKeys::AddKey(FKeyDetails(CyberithKeyNames::Orientation, LOCTEXT("Cyberith_Virtualizer_Orientation", "Cyberith Virtualizer - Absolute Orientation"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, "Cyberith"));
#endif
}

FVirtInputDevice::~FVirtInputDevice()
{
	//// Log
	//UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FVirtInputDevice::~FVirtInputDevice()"));

	// Release virt device
	this->ReleaseVirtDevice();
}

void FVirtInputDevice::ReleaseVirtDevice() 
{
	// Release virt device
	if (m_virtDevice != nullptr)
	{
		// Stop haptics
		m_virtDevice->HapticStop();

		// Close device
		m_virtDevice->Close();

		// Notify event that we disconnect from the device
		if (OnVirtDeviceChanged.IsBound())
			OnVirtDeviceChanged.Broadcast(m_virtDevice, EVirtDeviceChangedType::Disconnect);

		// Clear vars
		CybSDKUtils::FreeObjectForGarbageCollection(m_virtDevice);
		m_virtDevice = nullptr;

		// Log
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] Successfully disconnected from Virtualizer device."));
	}
}

void FVirtInputDevice::Tick(float DeltaTime)
{
#if WITH_EDITOR
	bool m_bIsPieActive = (GEditor->bIsSimulatingInEditor == true || GEditor->PlayWorld != nullptr);
	if (!m_bIsPieActive)
		return;
#endif
	
	#if PLATFORM_WINDOWS 
	// Are we null or closed,
	if (m_virtDevice == nullptr || m_virtDevice->IsOpen() == false)
	{
		// Release
		this->ReleaseVirtDevice();

		// Init Virtualizer again using settings
		this->ReloadSettings(m_settings);
	}
	#elif PLATFORM_ANDROID
	if (m_virtDevice == nullptr)
	{
		// Release
		this->ReleaseVirtDevice();

		// Init Virtualizer again using settings
		this->ReloadSettings(m_settings);
	}
	#endif
}

void FVirtInputDevice::SendControllerEvents()
{
	// Stop updating this function if our device is either null or closed,
	if (m_virtDevice == nullptr || m_virtDevice->IsOpen() == false) 
		return;

	// Create user and device IDs
	FPlatformUserId PlatformUserId = FPlatformMisc::GetPlatformUserForUserIndex(0);
	FInputDeviceId InputDeviceId;
	InputDeviceId.CreateFromInternalId(0);
	
	// Forward / Right axis movement
	const FVector movement = m_virtDevice->GetMovementVector();
	MessageHandler->OnControllerAnalog(CyberithKeyNames::MoveForward, PlatformUserId, InputDeviceId, movement.X);
	MessageHandler->OnControllerAnalog(CyberithKeyNames::MoveRight, PlatformUserId, InputDeviceId, movement.Y);

	// Orientation, expressed in float (0 <-> 1)
	const float orientation = m_virtDevice->GetPlayerOrientation();
	MessageHandler->OnControllerAnalog(CyberithKeyNames::Orientation, PlatformUserId, InputDeviceId, orientation);
}

void FVirtInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FVirtInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
	return false;
}

void FVirtInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	if (ControllerId != 0)
		return;

	if (m_settings->HapticSource != EVirtDeviceHapticSource::ForceFeedback)
		return;

	if (m_virtDevice == nullptr || !m_virtDevice->IsOpen())
		return;

	Value *= 100;

	if (Value > 0)
	{
		m_virtDevice->HapticSetVolume(Value);
		m_virtDevice->HapticPlay();
	}
	else
	{
		m_virtDevice->HapticStop();
	}
}

void FVirtInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
	float value = FMath::Max(values.LeftLarge, values.RightLarge);
	SetChannelValue(ControllerId, FForceFeedbackChannelType::LEFT_LARGE, value);
}

bool FVirtInputDevice::ReloadUsingSettings()
{
	if (m_settings != nullptr)
	{
		return ReloadSettings(m_settings);
	}
	else
	{
		return false;
	}
}

bool FVirtInputDevice::ReloadSettings(UVirtInputSettings* settings)
{
	m_settings = settings;

//#if WITH_EDITOR
//	if (!m_bIsPieActive)
//		return true;
//#endif

	// Release device
	this->ReleaseVirtDevice();

	// Init using settings
	return InitVirtualizer(m_settings->DeviceType, m_settings->bCalibrateOnConnect);
}

TWeakObjectPtr<UVirtDevice> FVirtInputDevice::GetDevice() const
{
	return TWeakObjectPtr<UVirtDevice>(m_virtDevice);
}

bool FVirtInputDevice::IsDecoupled() const
{
	return m_bIsDecoupled;
}

bool FVirtInputDevice::IsHapticEnabled() const
{
	return m_settings->HapticSource == EVirtDeviceHapticSource::VirtHaptic;
}

#if WITH_EDITOR
void FVirtInputDevice::StartInEditor()
{
	//m_bIsPieActive = true;

	// Release device
	this->ReleaseVirtDevice();

	// Init 
	if (m_virtDevice == nullptr)
	{
		InitVirtualizer(m_settings->DeviceType, m_settings->bCalibrateOnConnect);
	}

	// If device found,
	if (m_virtDevice != nullptr)
	{
		// Open
		if (m_virtDevice->IsOpen() == false)
			m_virtDevice->Open();

		// If open,
		if (m_virtDevice->IsOpen() == true)
		{
			// Init haptics
			if (m_virtDevice->HasHaptic() == true)
			{
				m_virtDevice->HapticSetGain(3);
				m_virtDevice->HapticSetFrequency(100);
				m_virtDevice->HapticSetVolume(0);
				m_virtDevice->HapticPlay();
			}
		}
	}
}

void FVirtInputDevice::StopInEditor()
{
	//m_bIsPieActive = false;
	
	//if (m_virtDevice != nullptr)
	//{
	//	// If open,
	//	if (m_virtDevice->IsOpen() == true)
	//	{
	//		// Stop haptics
	//		if (m_virtDevice->HasHaptic() == true)
	//			m_virtDevice->HapticStop();
	// 
	//		// Close
	//		m_virtDevice->Close();
	//	}
	// 
	//	//// Free memory
	//	//FreeObjectForGarbageCollection(m_virtDevice);
	//	//m_virtDevice = nullptr;
	//}

	this->ReleaseVirtDevice();
}
#endif

void FVirtInputDevice::OnBleConnectionActive()
{
	if (OnVirtDeviceChanged.IsBound())
		OnVirtDeviceChanged.Broadcast(m_virtDevice, EVirtDeviceChangedType::Connect);
}

void FVirtInputDevice::SetIsDecoupled_Internal(bool isDecoupled)
{
	switch (m_settings->DecouplingType) 
	{ 
		case EDecouplingType::Automatic: 
			m_bIsDecoupled = isDecoupled;
		break;
		case EDecouplingType::ForceCoupled:
			m_bIsDecoupled = false;
			break;
		case EDecouplingType::ForceDecoupled:
			m_bIsDecoupled = true;
			break;
		default: ;
	}
}

bool FVirtInputDevice::InitVirtualizer(EVirtDeviceType type, bool calibrateOnConnect)
{
	m_virtDevice = CreateVirtualizer(type);

	if (m_virtDevice == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[CYBERITH] FCyberithPlugin : Virtualizer device not found..."));
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Virtualizer device found, connecting..."));

	CybSDKUtils::SaveObjectFromGarbageCollection(m_virtDevice);
#if PLATFORM_WINDOWS
	if (!m_virtDevice->Open())
	{
		UE_LOG(LogTemp, Error, TEXT("[CYBERITH] FCyberithPlugin : Failed to connect to Virtualizer device."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Successfully connected to Virtualizer device."));

	if (calibrateOnConnect)
	{
#if WITH_EDITOR
		m_virtDevice->ResetPlayerOrientation();
		m_virtDevice->ResetPlayerHeight();

#else
		//Wait for 25ms to let the Device gather data before resetting it
		FTimerHandle OrientHandle, HeightHandle;
		m_virtDevice->GetWorld()->GetTimerManager().SetTimer(OrientHandle, m_virtDevice, &UVirtDevice::ResetPlayerOrientation, 0.025f, false);
		m_virtDevice->GetWorld()->GetTimerManager().SetTimer(HeightHandle, m_virtDevice, &UVirtDevice::ResetPlayerHeight, 0.025f, false);
#endif
	}

	if (OnVirtDeviceChanged.IsBound())
		OnVirtDeviceChanged.Broadcast(m_virtDevice, EVirtDeviceChangedType::Connect);
#endif
	return true;
}

UVirtDevice* FVirtInputDevice::CreateVirtualizer(EVirtDeviceType type)
{
	switch (type)
	{
	case EVirtDeviceType::Automatic:
		for (uint8 i = static_cast<uint8>(EVirtDeviceType::NativeVirtualizer); i <= static_cast<uint8>(EVirtDeviceType::Custom); ++i)
		{
			UVirtDevice* device = CreateVirtualizer(static_cast<EVirtDeviceType>(i));
			if (device != nullptr)
				return device;
		}
		return nullptr;
	case EVirtDeviceType::NativeVirtualizer:
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Initializing Standard Virtualizer Device"));
		this->SetIsDecoupled_Internal(true);
		return UVirt::FindDevice();
#if PLATFORM_WINDOWS
	case EVirtDeviceType::XInput:
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Initializing XInput Mockup"));
		this->SetIsDecoupled_Internal(false);
		return UVirt::CreateDeviceMockupXInput();
	case EVirtDeviceType::Keyboard:
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Initializing Keyboard Mockup"));
		this->SetIsDecoupled_Internal(false);
		return UVirt::CreateDeviceMockupKeyboard();
#endif
	case EVirtDeviceType::Custom:
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCyberithPlugin : Initializing Custom Mockup"));
		this->SetIsDecoupled_Internal(true);
		//ToDo: Add your custom UVirtDevice implementation here
		return nullptr;
	default:
		UE_LOG(LogTemp, Error, TEXT("[CYBERITH] FCyberithPlugin : No UVirtDevice implementation available, fallback not possible."));
		return nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
