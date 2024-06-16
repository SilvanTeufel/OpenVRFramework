// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BleConnectionWidget.h"
#include "CalibrationManager.h"
#include "Components/WidgetComponent.h"
#include "UObject/ScriptInterface.h"
#include "Cyberith/Standalone/StandaloneVirtDeviceInterface.h"
#include "Cyberith/Standalone/BleConnectionWidget.h"
#include "BleConnectionWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UBleConnectionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	UBleConnectionWidget* BleConnectionWidget;

	UFUNCTION()
	void ActivateWidget();
	UFUNCTION()
	void DeactivateWidget();
	
	TScriptInterface<IStandaloneVirtDeviceInterface> m_standaloneVirtDeviceInterface;
	FString m_savedVirtSamName;
	bool m_calibrationRequired = false;

private:
	UFUNCTION()
	void OnWidgetNativeConstruct();
	
	UFUNCTION()
	void OnBleStateChanged(EBleConnectionStates state);
	
	UFUNCTION()
	void OnReconnectClicked();
	
	UFUNCTION()
	void OnPairedDeviceClicked(FString PairedDeviceName);

	void SwitchWidgetObjectsHolderVisibility(UCanvasPanel* ObjectsHolder);

	UPROPERTY()
	UCalibrationManager* m_calibrationManager;
};
