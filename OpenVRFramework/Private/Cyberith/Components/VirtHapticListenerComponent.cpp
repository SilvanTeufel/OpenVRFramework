// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/Components/VirtHapticListenerComponent.h"
#include "Cyberith/Components/VirtHapticEmitterComponent.h"
#include "GameFramework/Actor.h"
#include "Cyberith/CybSDK/VirtDevice.h"
#include "OpenVRFramework.h"
#include "Cyberith/Input/VirtInputDevice.h"

// Sets default values for this component's properties
UVirtHapticListenerComponent::UVirtHapticListenerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	bAutoActivate = true;

	// ...
}

void UVirtHapticListenerComponent::AddEmitter(UVirtHapticEmitterComponent* emitter)
{
	m_emitters.Add(emitter);
}

void UVirtHapticListenerComponent::RemoveEmitter(UVirtHapticEmitterComponent* emitter)
{
	m_emitters.RemoveSwap(emitter);

	if (m_emitters.Num() == 0)
	{
		TWeakObjectPtr<UVirtDevice> virtDevice = m_deviceController->GetDevice();

		if (!virtDevice.IsValid() || !virtDevice->IsOpen())
			return;

		if (!virtDevice->HasHaptic())
			return;

		virtDevice->HapticSetVolume(0);
	}
}

void UVirtHapticListenerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	m_deviceController = FOpenVRFrameworkModule::GetVirtualizerInputDevice();
}

void UVirtHapticListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!m_deviceController.IsValid())
		return;

	TWeakObjectPtr<UVirtDevice> virtDevice = m_deviceController->GetDevice();

	if (!virtDevice.IsValid() || !virtDevice->IsOpen())
		return;

	if (virtDevice->HasHaptic())
	{
		virtDevice->HapticStop();
	}
}

void UVirtHapticListenerComponent::OnVirtDeviceChanged(UVirtDevice* virtDevice, EVirtDeviceChangedType callbackType)
{
	if (!m_deviceController.IsValid() || !m_deviceController->IsHapticEnabled())
		return;

	if (!virtDevice->HasHaptic())
		return;

	switch (callbackType)
	{
	case EVirtDeviceChangedType::Connect:
		virtDevice->HapticSetGain(3);
		virtDevice->HapticSetFrequency(0);
		virtDevice->HapticSetVolume(0);
		virtDevice->HapticPlay();
		break;

	case EVirtDeviceChangedType::Disconnect:
		virtDevice->HapticStop();
		break;
	}
}

float UVirtHapticListenerComponent::SumUpDecibel(float a, float b)
{
	float powA = FMath::Pow(a / 10.0f, 10.0f);
	float powB = FMath::Pow(b / 10.0f, 10.0f);

	float powSum = powA + powB;
	return 10.0f * FMath::LogX(10, powSum);
}


// Called every frame
void UVirtHapticListenerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FCybSDK_PluginModule::GetVirtualizerInputDevice() will be initialized by the Unreal Engine in the first player tick
	if (!m_deviceController.IsValid())
	{
		m_deviceController = FOpenVRFrameworkModule::GetVirtualizerInputDevice();

		if (!m_deviceController.IsValid())
			return;

		m_deviceController->OnVirtDeviceChanged.AddDynamic(this, &UVirtHapticListenerComponent::OnVirtDeviceChanged);

		TWeakObjectPtr<UVirtDevice> virtDevice = m_deviceController->GetDevice();
		
		// Manually trigger connection event if a device is already connected
		if (virtDevice.IsValid() && virtDevice->IsOpen())
			OnVirtDeviceChanged(virtDevice.Get(), EVirtDeviceChangedType::Connect);
	}


	TWeakObjectPtr<UVirtDevice> virtDevice = m_deviceController->GetDevice();

	if (!virtDevice.IsValid() || !virtDevice->IsOpen())
		return;
	
	if (!virtDevice->HasHaptic())
		return;

	if (m_emitters.Num() == 0)
		return;
	
	float sumForce = 0;
	float sumFrequency = 0;
	int emittersInRange = 0;

	for(UVirtHapticEmitterComponent* emitter : m_emitters)
	{
		float distance = FVector::Distance(GetOwner()->GetActorLocation(), emitter->GetComponentLocation());
		if (distance < m_maxRange && (distance < emitter->GetRange() || emitter->GetKeepActive()))
		{
			float force = emitter->EvaluateForce(GetOwner()->GetActorLocation()) * 100;
			sumForce += force;

			sumFrequency += emitter->GetFrequency();

			++emittersInRange;
		}
	}
	
	if (emittersInRange == 0)
	{
		virtDevice->HapticSetVolume(0);
		return;
	}

	sumFrequency /= emittersInRange;

	//Haptic Unit has resonance frequency between 35 - 45Hz. Therefore this frequencies are filtered out and set to 34 or 46Hz respectively
	if ((int)sumFrequency >= 35 && (int)sumFrequency <= 39)
		sumFrequency = 34;
	if ((int)sumFrequency >= 40 && (int)sumFrequency <= 45)
		sumFrequency = 46;

	virtDevice->HapticSetFrequency(sumFrequency);
	virtDevice->HapticSetVolume((int)sumForce / emittersInRange);
}

