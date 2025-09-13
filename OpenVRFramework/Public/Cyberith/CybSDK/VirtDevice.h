// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "VirtDeviceInfo.h"
#if PLATFORM_ANDROID
#include "Android/VirtDevice_Android.h"
#endif
#include "Cyberith/Standalone/StandaloneVirtDeviceInterface.h"
#include "VirtDevice.generated.h"

class UVirtDevice;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EVirtDeviceChangedType : uint8
{
	Connect 			UMETA(DisplayName = "On Connect"),
	Disconnect			UMETA(DisplayName = "On Disconnect")
};

//https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Delegates/Multicast/index.html
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVirtDeviceChangedCallback, UVirtDevice*, virtDevice, EVirtDeviceChangedType, callbackType);

namespace CybSDK {
	#if PLATFORM_WINDOWS
	class VirtDevice;
	typedef class VirtDevice NativeVirtDevice;
	#elif PLATFORM_ANDROID
	typedef UVirtDevice_Android NativeVirtDevice;
	#endif
}



/**
 * 
 */
UCLASS(ClassGroup = (Virtualizer), BlueprintType, Blueprintable)
class OPENVRFRAMEWORK_API UVirtDevice : public UObject
{
	GENERATED_BODY()

public:
	UVirtDevice();
	~UVirtDevice();

	static UVirtDevice* WrapVirtDevice(CybSDK::NativeVirtDevice* device);

	const CybSDK::NativeVirtDevice* GetNativeDevice() const;
	CybSDK::NativeVirtDevice* GetNativeDevice();
	
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	TScriptInterface<IStandaloneVirtDeviceInterface> GetStandaloneVirtDeviceInterface();

	//****************************************************************************************
	//* Connection
	//****************************************************************************************

	/**
	 * @brief Opens the connection to the Virtualizer device.
	 * @remark No other application can open this device at the same time.
	 * @return true if the connection was successfully opened, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Connection")
		bool Open();
	/**
	 * @brief Checks if the connection was opened before.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Connection")
		bool IsOpen() const;
	/**
	 * @brief Closes the connection to the Virtualizer device.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Connection")
		void Close();


	//****************************************************************************************
	//* Device Infos
	//****************************************************************************************

	/**
	 * @brief Returns the USB infos of this device.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Device Infos")
		const FVirtDeviceInfo& GetDeviceInfo() const;

	//****************************************************************************************
	//* Virtualizer Game Data
	//****************************************************************************************

	/**
	 * @brief Returns the current player height relative to the default height.
	 * @remark The default height is set by the ResetPlayerHeight method.
	 * @remark height < -threshold: crouching
	 * @remark height >  threshold: jumping
	 * @return 1.00f = 1cm.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		float GetPlayerHeight() const;

	/**
	 * @brief Assigns the current height to the default height.
	 * @remark This method should be called while the player is asked to stand upright.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		void ResetPlayerHeight();

	/**
	 * @brief Returns the orientation of the player as an absolute value.
	 * @remark The origin is set by the ResetPlayerOrientation method and increases clockwise.
	 * @return logical: 0.00f to 1.00f (= physical: 0° to 360°).
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		float GetPlayerOrientation() const;

	/**
	 * @brief Assigns the current orientation to the default vector.
	 * @remark This method should be called while the player is asked to look forward.
	 * @remark This orientation should be used to calibrate the HMD.
	 * @deprecated Unused as new Virtualizers orient themselves automatically.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		void ResetPlayerOrientation();

	/**
	 * @brief Returns the orientation of the player as vector.
	 * @remark The origin is set by the ResetPlayerOrientation method and increases clockwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		FVector GetPlayerOrientationVector() const;

	/**
	 * @brief Returns the orientation of the player as quaternion.
	 * @remark The origin is set by the ResetPlayerOrientation method and increases clockwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		FQuat GetPlayerOrientationQuaternion() const;

	/**
	 * @brief Returns the current movement speed in meters per second.
	 * @return 1.00f = 1m/s
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		float GetMovementSpeed() const;

	/**
	 * @brief Returns the movement direction relative to the current player orientation.
	 * @remark The origin is the GetPlayerOrientation method and increases clockwise.
	 * @return logical: -1.00f to 1.00f (= physical: -180° to 180°).
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		float GetMovementDirection() const;

	/**
	 * @brief Returns the movement direction as vector relative to the current player orientation.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		FVector GetMovementVector() const;

	/**
	 * @brief Returns the movement direction as a speed scaled vector relative to the current player orientation.
	 * @remark The origin is the GetPlayerOrientation method and increases clockwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Game Data")
		FVector GetMovementDirectionVector() const;

	//****************************************************************************************
	//* Haptic
	//****************************************************************************************

	/**
	 * @brief Checks if the Virtualizer device supports haptic feedback.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		bool HasHaptic() const;

	/**
	 * @brief Play a signal on the haptic unit.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void HapticPlay();

	/**
	 * @brief Stop the haptic unit.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void HapticStop();

	/**
	 * @brief Set the gain (dB) level of the haptic unit.
	 * @param gain The value can be 0, 1, 2 or 3.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void HapticSetGain(int gain);

	/**
	 * @brief Set the frequency (Hz) of a sine wave on the haptic unit.
	 * @param frequency The value is valid between 10Hz and 80Hz.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void HapticSetFrequency(int frequency);

	/**
	 * @brief Sets the haptic feedback (change of amplitude) in the baseplate.
	 * @param volume The value is valid between 0 (no feedback) and 100 (full feedback).
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Device|Haptic")
		void HapticSetVolume(int volume);

protected:
	void Init(CybSDK::NativeVirtDevice* nativeDevice);
	
	CybSDK::NativeVirtDevice* m_device = nullptr;

	UPROPERTY()
	FVirtDeviceInfo m_fDeviceInfo;
};
