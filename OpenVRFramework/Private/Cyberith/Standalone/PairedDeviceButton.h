// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PairedDeviceButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPairedDeviceButtonClickedEvent, FString, DeviceName);

/**
 * 
 */
UCLASS()
class UPairedDeviceButton : public UButton
{
	GENERATED_BODY()
public:
	UPairedDeviceButton(const FObjectInitializer& ObjectInitializer);
	~UPairedDeviceButton();

	virtual void OnWidgetRebuilt() override;
	
	void SetDeviceName(FString Name);
	
	UPROPERTY()
	FOnPairedDeviceButtonClickedEvent OnButtonClicked;
	
	FString mDeviceName = "VirtSAM1";

private:
	UFUNCTION()
	void OnClickedWithParam();

	UPROPERTY()
	UTextBlock* ButtonLabel;
};
