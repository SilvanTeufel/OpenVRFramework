// Fill out your copyright notice in the Description page of Project Settings.

#include "Cyberith/Standalone/BleConnectionWidgetComponent.h"
#include "OpenVRFramework.h"
#include "Cyberith/Standalone/StandaloneVirtDeviceInterface.h"
#include "PairedDeviceButton.h"
#include "VRNotificationsComponent.h"

void UBleConnectionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	BleConnectionWidget = Cast<UBleConnectionWidget>(GetUserWidgetObject());
	BleConnectionWidget->NativeConstructDelegate.BindUFunction(this, FName("OnWidgetNativeConstruct"));
}

void UBleConnectionWidgetComponent::OnWidgetNativeConstruct()
{
	m_calibrationManager = GetOwner()->FindComponentByClass<UCalibrationManager>();
	
	m_standaloneVirtDeviceInterface = FOpenVRFrameworkModule::GetVirtualizerInputDevice()->GetDevice()->GetStandaloneVirtDeviceInterface();
	m_standaloneVirtDeviceInterface->OnBleStateChanged.AddDynamic(this, &UBleConnectionWidgetComponent::OnBleStateChanged);
	OnBleStateChanged(m_standaloneVirtDeviceInterface->GetCurrentState());
	BleConnectionWidget->ReconnectButton->OnClicked.AddDynamic(this, &UBleConnectionWidgetComponent::OnReconnectClicked);

	m_savedVirtSamName = m_standaloneVirtDeviceInterface->GetSavedVirtSamName();
	BleConnectionWidget->ConnectionText->SetText(FText::FromString(FString::Printf(TEXT("Setting up VirtSAM connection with '%s', please wait..."), *m_savedVirtSamName)));

	/*if (BleConnectionWidget->CalibrationObjectsHolder != nullptr)
	{
		m_calibrationRequired = true;
		if (BleConnectionWidget->CalibrateButton != nullptr) // Calibration is done with a UI button
			{
			BleConnectionWidget->CalibrateButton->OnClicked.AddDynamic(this, &UBleConnectionWidgetComponent::OnCalibration);
			BleConnectionWidget->CloseCalibrationButton->OnClicked.AddDynamic(this, &UBleConnectionWidgetComponent::DeactivateWidget);
			}
		else // Calibration is done using the recenter event of the HMD
			{
			UVRNotificationsComponent* VRNotificationsComponent = NewObject<UVRNotificationsComponent>(GetOuter(), UVRNotificationsComponent::StaticClass(), FName("VRNotifications"));
			VRNotificationsComponent->RegisterComponent();
			VRNotificationsComponent->VRControllerRecenteredDelegate.AddDynamic(this, &UBleConnectionWidgetComponent::OnCalibration);
			}

		BleConnectionWidget->GetOwningPlayer()->InputComponent->BindAction("CalibrateStandaloneVirt", IE_Pressed, this, &UBleConnectionWidgetComponent::ToggleCalibrationWindow);
	}*/
}

/*void UBleConnectionWidgetComponent::ToggleCalibrationWindow()
{
	if (m_calibrationRequired) {
		if (m_standaloneVirtDeviceInterface->GetCurrentState() == Active)
		{
			if (BleConnectionWidget->GetVisibility() == ESlateVisibility::Collapsed) {
				ActivateWidget();
			} else if (BleConnectionWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
			{
				DeactivateWidget();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::ToggleCalibrationWindow : Calibration is not used in this BleConnectionWidget"));
	}
		
}*/

void UBleConnectionWidgetComponent::OnBleStateChanged(EBleConnectionStates state)
{
	switch (state)
	{
		case Unknown:
		{
			SwitchWidgetObjectsHolderVisibility(BleConnectionWidget->ConnectionObjectsHolder);
			BleConnectionWidget->ConnectionText->SetText(FText::FromString(FString::Printf(TEXT("ERROR: Possibly, no Android permission has been given yet, restart app"))));
			BleConnectionWidget->StatusText->SetText(FText::FromString(TEXT("Unknown")));
			UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::OnBleStateChanged : BLE Connection State is 'Unknown'"));
			break;
		}
		case Selecting:
		{
			SwitchWidgetObjectsHolderVisibility(BleConnectionWidget->SelectingObjectsHolder);
	
			TArray<FString> pairedDevices = m_standaloneVirtDeviceInterface->GetPairedDevices();
			if (pairedDevices.Num() == 1)
			{
				BleConnectionWidget->SelectingText->SetText(FText::FromString(TEXT("This headset is not paired with any device. Please, pair first with the Virtualizer and restart this app.")));
				BleConnectionWidget->SelectingButtonsScrollBox->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				m_savedVirtSamName = m_standaloneVirtDeviceInterface->GetSavedVirtSamName();
				BleConnectionWidget->SelectingText->SetText(FText::FromString(FString::Printf(TEXT("The VirtSAM with the name '%s' could not be found in your paired devices. Please, select another device or pair again:"), *m_savedVirtSamName)));

				for (int i = 1; i < pairedDevices.Num(); i++)
				{
					UPairedDeviceButton* PairedDeviceButton;
					//UTextBlock* ButtonLabel;

					PairedDeviceButton = NewObject<UPairedDeviceButton>(BleConnectionWidget->SelectingButtonsScrollBox);
					BleConnectionWidget->SelectingButtonsScrollBox->AddChild(PairedDeviceButton);
					/*ButtonLabel = NewObject<UTextBlock>(PairedDeviceButton);
					PairedDeviceButton->AddChild(ButtonLabel);*/

					PairedDeviceButton->SetDeviceName(pairedDevices[i]);

					PairedDeviceButton->OnButtonClicked.AddDynamic(this, &UBleConnectionWidgetComponent::OnPairedDeviceClicked);
				}
			}
			break;
		}
		case Connecting:
		{
			SwitchWidgetObjectsHolderVisibility(BleConnectionWidget->ConnectionObjectsHolder);
	
			m_savedVirtSamName = m_standaloneVirtDeviceInterface->GetSavedVirtSamName();
			BleConnectionWidget->ConnectionText->SetText(FText::FromString(FString::Printf(TEXT("Setting up VirtSAM connection with '%s', please wait..."), *m_savedVirtSamName)));
			BleConnectionWidget->StatusText->SetText(FText::FromString(TEXT("Connecting")));
			break;
		}
		case Connection_Failed:
		{
			ActivateWidget();
			SwitchWidgetObjectsHolderVisibility(BleConnectionWidget->FailedObjectsHolder);
	
			BleConnectionWidget->FailedText->SetText(FText::FromString(FString::Printf(TEXT("Connection with '%s' failed!"), *m_savedVirtSamName)));
			break;
		}
		case Connected:
		{
			BleConnectionWidget->StatusText->SetText(FText::FromString(TEXT("Connected")));
			break;
		}
		case Initializing_Data:
		{
			BleConnectionWidget->StatusText->SetText(FText::FromString(TEXT("Initializing Data")));
			break;
		}
		case Active:
		{
			if (m_calibrationManager != nullptr)
			{
				SwitchWidgetObjectsHolderVisibility(BleConnectionWidget->CalibrationObjectsHolder);
				ECalibrationDataResult calibrationDataResult = m_calibrationManager->GetCalibrationDataResult();

				switch (calibrationDataResult)
				{
					case Success:
					{
						DeactivateWidget();
						BleConnectionWidget->CalibrationText->SetText(FText::FromString(TEXT("Calibration finished!")));
						break;
					}
					case Exception:
					{
						BleConnectionWidget->CalibrationText->SetText(FText::FromString(TEXT("Calibration error. Please, close this app and redo the calibration in the Cyberith Calibration app.\n\n If the issue persists, please contact us at support@cyberith.com")));
						UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::OnBleStateChanged : Calibration data result is 'Exception'"));
						break;
					}
					case NoCalibrationApp:
					{
						BleConnectionWidget->CalibrationText->SetText(FText::FromString(TEXT("Calibration app missing! Please, install the Cyberith Calibration app on your VR device.\n\nYou can find this app on the Cyberith Developer Center.")));
						UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::OnBleStateChanged : Calibration data result is 'NoCalibrationApp'"));
						break;
					}
					case NoCalibrationData:
					{
						BleConnectionWidget->CalibrationText->SetText(FText::FromString(TEXT("Calibration missing! Please, close this app and perform a calibration in the Cyberith Calibration app.")));
						UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::OnBleStateChanged : Calibration data result is 'NoCalibrationData'"));
						break;
					}
					case BoundaryNotConfigured:
					{
						BleConnectionWidget->CalibrationText->SetText(FText::FromString(TEXT("Boundary not configured. Please, configure the boundaries in the Settings.\n\nThe boundary is essential for calibrating the correct forward direction of the Virtualizer.")));
						UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] BleConnectionWidgetComponent::OnBleStateChanged : Calibration data result is 'BoundaryNotConfigured'"));
						break;
					}
				}
			}
			else
			{
			DeactivateWidget();
			}
			break;
		}
	}
}

void UBleConnectionWidgetComponent::OnReconnectClicked()
{
	m_standaloneVirtDeviceInterface->FindAndConnectSavedVirtSam();
}

void UBleConnectionWidgetComponent::OnPairedDeviceClicked(FString PairedDeviceName)
{
	m_standaloneVirtDeviceInterface->SaveToFileVirtSam(PairedDeviceName);
	m_standaloneVirtDeviceInterface->FindAndConnectSavedVirtSam();
}

void UBleConnectionWidgetComponent::SwitchWidgetObjectsHolderVisibility(UCanvasPanel* ObjectsHolder)
{
	for (UCanvasPanel* Holder : BleConnectionWidget->ObjectsHolders)
	{
		if (Holder != nullptr)
		{
			if (Holder == ObjectsHolder)
			{
				Holder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				Holder->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UBleConnectionWidgetComponent::ActivateWidget()
{
	BleConnectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBleConnectionWidgetComponent::DeactivateWidget()
{
	BleConnectionWidget->SetVisibility(ESlateVisibility::Collapsed);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
