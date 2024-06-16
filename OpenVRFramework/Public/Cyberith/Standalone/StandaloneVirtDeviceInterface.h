// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StandaloneVirtDeviceInterface.generated.h"

UENUM(BlueprintType)
enum EBleConnectionStates
{
	Unknown,
	Selecting,
	Connecting,
	Connection_Failed,
	Connected,
	Initializing_Data,
	Active
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBleStateChangedDelegate, EBleConnectionStates, State);

// This class does not need to be modified.
UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class OPENVRFRAMEWORK_API UStandaloneVirtDeviceInterface : public UInterface
{
	GENERATED_BODY()
};

class OPENVRFRAMEWORK_API IStandaloneVirtDeviceInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns a list of names of all the paired Bluetooth devices.
	 * @remark The first index is a boolean depending on if the saved VirtSAM name is in the list of paired devices.
	 * @remark This method can be used to let the user identify the VirtSAM from the list.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual TArray<FString> GetPairedDevices() = 0;

	/**
	 * @brief Returns the Bluetooth name of the VirtSAM device that is currently saved and used for connecting.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual FString GetSavedVirtSamName() = 0;

	/**
	 * @brief Saves a given Name to a local file.
	 * @remark This name is being used to connect to the VirtSAM.
	 * @param Name The VirtSAM Bluetooth name that can be used to connect to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual void SaveToFileVirtSam(FString Name) = 0;

	/**
	 * @brief Searches if the saved name of the VirtSAM is in the list of paired devices.
	 * If so, the connection process is started. Otherwise, the state is set to Selecting.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual void FindAndConnectSavedVirtSam() = 0;

	/**
	 * @brief Returns the current state of the Ble Connection process.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual EBleConnectionStates GetCurrentState() = 0;
	
	/**
	 * @brief Binds a given function to the OnBleStateChanged delegate
	 * @remark Specifically for use in Blueprints, not C++
	 * @param TargetObject Reference to the object that the named function is a member of.
	 * @param FuncName Name of the function that needs to be bound to the delegate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	virtual void BindToBleStateChangedDelegate(UObject* TargetObject, FName FuncName) = 0;

	UFUNCTION()
	virtual void SetOrientationOffset(float offset) = 0;
	
	FOnBleStateChangedDelegate OnBleStateChanged;
};
