// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "IInputDevice.h"
#include "VirtInputSettings.h"
#include "Cyberith/CybSDK/VirtDevice.h"

/**
 *
 */
class OPENVRFRAMEWORK_API FVirtInputDevice : public IInputDevice
{
public:
#pragma region IInputDevice

	FVirtInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~FVirtInputDevice();

	void ReleaseVirtDevice();

	void Tick(float DeltaTime) override;

	void SendControllerEvents() override;

	void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;

	void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values) override;

	bool ReloadUsingSettings();

	bool ReloadSettings(UVirtInputSettings* settings);

	bool IsGamepadAttached() const override
	{
		return m_virtDevice != nullptr;
	}

#pragma endregion IInputDevice

#pragma region VirtDeviceController

	// Returns the Virtualizer device managed by the VirtDeviceController.
	TWeakObjectPtr<UVirtDevice> GetDevice() const;

	// Returns true if the VirtDevice supports decoupled movement, otherwise false.
	bool IsDecoupled() const;

	// Returns true if the VirtDevice haptic is enabled, otherwise false.
	bool IsHapticEnabled() const;

	UPROPERTY(Category = "Virtualizer", BlueprintAssignable)
		FOnVirtDeviceChangedCallback OnVirtDeviceChanged;

	void OnBleConnectionActive();
	
#if WITH_EDITOR
	void StartInEditor();

	void StopInEditor();
#endif

#pragma endregion VirtDeviceController

private:
	void SetIsDecoupled_Internal(bool isDecoupled);

	bool InitVirtualizer(EVirtDeviceType type, bool calibrateOnConnect);
	UVirtDevice* CreateVirtualizer(EVirtDeviceType type);

	UPROPERTY()
		UVirtInputSettings* m_settings = nullptr;
	UPROPERTY()
		UVirtDevice* m_virtDevice = nullptr;
	bool m_bIsDecoupled = true;
//#if WITH_EDITOR
//	bool m_bIsPieActive = false;
//#endif

	/* Message handler */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	
};
