// Fill out your copyright notice in the Description page of Project Settings.


#include "PairedDeviceButton.h"
#include "Components/ButtonSlot.h"

UPairedDeviceButton::UPairedDeviceButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnClicked.AddDynamic(this, &UPairedDeviceButton::OnClickedWithParam);
}

UPairedDeviceButton::~UPairedDeviceButton()
{
}

void UPairedDeviceButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	ButtonLabel = NewObject<UTextBlock>(this);
	AddChild(ButtonLabel);
	UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ButtonLabel->Slot);
	ButtonSlot->SetPadding({0, 20});
}

void UPairedDeviceButton::SetDeviceName(FString Name)
{
	mDeviceName = Name;
	ButtonLabel->SetText(FText::FromString(Name));
}

void UPairedDeviceButton::OnClickedWithParam()
{
	OnButtonClicked.Broadcast(mDeviceName);
}
