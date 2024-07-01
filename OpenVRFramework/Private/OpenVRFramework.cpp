// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.
// Copyright 2024 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "OpenVRFramework.h"



#include "OpenVRFramework.h"
#include "Runtime/Core/Public/CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"

// Input
#if PLATFORM_WINDOWS
#include "ThirdParty/CybSDK/CVirt.h"
#endif
#include "Cyberith/Input/VirtInputDevice.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

// Settings
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Cyberith/Input/VirtInputSettings.h"

#if WITH_EDITOR
#include "Cyberith/Components/VirtHapticEmitterComponent.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#endif

#define LOCTEXT_NAMESPACE "FOpenVRFrameworkModule"

TSharedPtr<IInputDevice> FOpenVRFrameworkModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCybSDK_PluginModule: Created new input device!"));

	// See VirtInputDevice.h for the definition of the IInputDevice we are returning here
	m_virtInputDevice = MakeShareable(new FVirtInputDevice(InMessageHandler));

	return m_virtInputDevice;
}

void FOpenVRFrameworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IInputDeviceModule::StartupModule();

	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCybSDK_PluginModule: StartupModule"));
#if PLATFORM_WINDOWS
	if (!LoadNativeDll()) 
		return;

	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCybSDK_PluginModule: StartupModule > Found native DLL"));
#endif
	RegisterSettings();

	RegisterInputDevice();

	RegisterVisualizer();
}

void FOpenVRFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnregisterVisualizer();

	UnregisterInputDevice();

	UnregisterSettings();

	UnloadNativeDll();

	IInputDeviceModule::ShutdownModule();
}

bool FOpenVRFrameworkModule::LoadNativeDll()
{
#if PLATFORM_WINDOWS
	// Get the base directory of this plugin
	const FString pluginBaseDir = IPluginManager::Get().FindPlugin("OpenVRFramework")->GetBaseDir();
	const FString nativeBaseDir1 = FPaths::Combine(*pluginBaseDir, TEXT("Binaries/ThirdParty/CybSDK"));
	const FString nativeBaseDir2 = FPaths::Combine(*pluginBaseDir, TEXT("Source/ThirdParty/CybSDK"));

	// Function to construct the library path based on the base directory
	auto GetLibraryPath = [](const FString& baseDir) -> FString
	{
#if PLATFORM_64BITS
		return FPaths::Combine(*baseDir, TEXT("Win64/CybSDK.dll"));
#else
		return FPaths::Combine(*baseDir, TEXT("Win32/CybSDK.dll"));
#endif
	};

	// Attempt to load the DLL from the first location
	FString libraryPath = GetLibraryPath(nativeBaseDir1);
	m_cybSDKLibraryHandle = !libraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*libraryPath) : nullptr;

	// If the first attempt fails, try the second location
	if (!m_cybSDKLibraryHandle)
	{
		libraryPath = GetLibraryPath(nativeBaseDir2);
		m_cybSDKLibraryHandle = !libraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*libraryPath) : nullptr;
	}

	// Check if the DLL was loaded successfully
	if (!m_cybSDKLibraryHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("[CYBERITH] FCyberithPlugin: Failed to load CybSDK.dll"));
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FCyberithPlugin_FailedLoad", "[CYBERITH] FCyberithPlugin: Failed to load CybSDK.dll"));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("[CYBERITH] FCybSDK_PluginModule: Loaded CybSDK V%d.%d"), CybSDK::Virt::GetSDKVersion() >> 8, CybSDK::Virt::GetSDKVersion() & 0xFF);
	return true;

#else
	UE_LOG(LogTemp, Error, TEXT("[CYBERITH] FCybSDK_PluginModule: Unable to load CybSDK Dll on Platforms other than Windows!"));
	return false;
#endif
}

void FOpenVRFrameworkModule::UnloadNativeDll()
{
	// Free the dll handle
	FPlatformProcess::FreeDllHandle(m_cybSDKLibraryHandle);
	m_cybSDKLibraryHandle = nullptr;
}

void FOpenVRFrameworkModule::RegisterVisualizer()
{
#if WITH_EDITOR
	if (GUnrealEd != nullptr)
	{
		TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FVirtHapticEmitterVisualizer());

		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UVirtHapticEmitterComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
#endif
}

void FOpenVRFrameworkModule::UnregisterVisualizer()
{
#if WITH_EDITOR
	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UVirtHapticEmitterComponent::StaticClass()->GetFName());
	}
#endif
}

void FOpenVRFrameworkModule::RegisterInputDevice()
{
	// IMPORTANT: This line registers our input device module with the engine.
	//	      If we do not register the input device module with the engine,
	//	      the engine won't know about our existence. Which means 
	//	      CreateInputDevice never gets called, which means the engine
	//	      will never try to poll for events from our custom input device.
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

void FOpenVRFrameworkModule::UnregisterInputDevice()
{
	// Unregister our input device module
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

bool FOpenVRFrameworkModule::HandleSettingsSaved()
{
	UVirtInputSettings* Settings = GetMutableDefault<UVirtInputSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	if (!m_virtInputDevice->ReloadSettings(Settings))
	{
#if WITH_EDITOR
		FMessageDialog::Open(EAppMsgType::Ok, 			
			FText::FromString(FString::Printf(TEXT("[CYBERITH] Device Type '%ws' not available at this moment."), *GetEnumAsString(Settings->DeviceType))));
#endif
		return false;
	}

	return true;
}

void FOpenVRFrameworkModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "CybSDK",
			LOCTEXT("CybSDKSettingsName", "CybSDK"),
			LOCTEXT("CybSDKSettingsDescription", "Configure the CybSDK plugin"),
			GetMutableDefault<UVirtInputSettings>()
		);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FOpenVRFrameworkModule::HandleSettingsSaved);
		}
	}
}

void FOpenVRFrameworkModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
		// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "CybSDK");
	}
}



IMPLEMENT_MODULE(FOpenVRFrameworkModule, OpenVRFramework)