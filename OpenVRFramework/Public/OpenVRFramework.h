// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Runtime/InputDevice/Public/IInputDeviceModule.h"
#include "Cyberith/Input/VirtInputDevice.h"

class FOpenVRFrameworkModule : public IInputDeviceModule
{
public:

	/** Creates a new instance of the IInputDevice associated with this IInputDeviceModule **/
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	/** Called right after the module DLL has been loaded and the module object has been created **/
	virtual void StartupModule() override;

	/** Called before the module is unloaded, right before the module object is destroyed. **/
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FOpenVRFrameworkModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FOpenVRFrameworkModule>("CybSDK_Plugin");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CybSDK_Plugin");
	}

	inline TSharedPtr<FVirtInputDevice> GetInputDevice() const
	{
		return m_virtInputDevice;
	}

	static TSharedPtr<FVirtInputDevice> GetVirtualizerInputDevice()
	{
		if (!IsAvailable())
		{
			return nullptr;
		}

		return Get().GetInputDevice();
	}

private:
	bool LoadNativeDll();
	void UnloadNativeDll();
	/** Handle to the loaded Cyberith Virtualizer API DLL */
	void* m_cybSDKLibraryHandle = nullptr;

	void RegisterVisualizer();
	void UnregisterVisualizer();

	void RegisterInputDevice();
	void UnregisterInputDevice();
	TSharedPtr<FVirtInputDevice> m_virtInputDevice;

	// Callback for when the settings were saved.
	bool HandleSettingsSaved();
	void RegisterSettings();
	void UnregisterSettings();
};
