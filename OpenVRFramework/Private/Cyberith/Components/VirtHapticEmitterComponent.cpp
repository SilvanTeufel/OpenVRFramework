// Copyright 2014-2018 Cyberith GmbH. All Rights Reserved.

#include "Cyberith/Components/VirtHapticEmitterComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "UObject/UObjectIterator.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Cyberith/Components/VirtHapticListenerComponent.h"
#include "Particles/Location/ParticleModuleLocationPrimitiveSphere.h"

// Sets default values for this component's properties
UVirtHapticEmitterComponent::UVirtHapticEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;

	// ...
}


void UVirtHapticEmitterComponent::Play()
{
	m_timeStart = UGameplayStatics::GetUnpausedTimeSeconds(this);

	if (m_bIsPlaying)
		return;

	m_bIsPlaying = true;

	if (m_hapticListener != nullptr)
		m_hapticListener->AddEmitter(this);
}

void UVirtHapticEmitterComponent::Stop()
{
	if (!m_bIsPlaying)
		return;

	m_bIsPlaying = false;

	if (m_hapticListener != nullptr)
		m_hapticListener->RemoveEmitter(this);
}

float UVirtHapticEmitterComponent::EvaluateForce(FVector listenerPosition)
{
	if (!IsActive() || !m_bIsPlaying)
		return 0;

	float timeDelta = (UGameplayStatics::GetUnpausedTimeSeconds(this) - m_timeStart);
	float timeInLoop = FMath::Fmod(timeDelta, m_duration);

	//Evaluate time
	float forceTimeP = m_volumeOverTime->GetFloatValue(timeInLoop / m_duration);
	
	//Evaluate distance
	FVector v1 = GetComponentLocation();
	v1.Z = 0.0;
	FVector v2 = listenerPosition;
	v2.Z = 0.0;
	float dist = FVector::Dist(v1, v2) / m_range;
	float forceDistP = m_forceOverDistance->GetFloatValue(FMath::Clamp(dist, 0.0f, 1.0f));
	
	return forceTimeP * forceDistP;
}

float UVirtHapticEmitterComponent::GetRange() const
{
	return m_range;
}

float UVirtHapticEmitterComponent::GetDuration() const
{
	return m_duration;
}

float UVirtHapticEmitterComponent::GetFrequency() const
{
	return m_frequency;
}

float UVirtHapticEmitterComponent::GetKeepActive() const
{
	return m_bKeepActive;
}

void UVirtHapticEmitterComponent::SetKeepActive(bool keepActive)
{
	m_bKeepActive = keepActive;
}

void UVirtHapticEmitterComponent::SetAutoStartPlaying(bool autoStart)
{
	m_bAutoStartPlaying = autoStart;
}

// Called when the game starts
void UVirtHapticEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_hapticListener == nullptr)
	{
		for (TObjectIterator<UVirtHapticListenerComponent> itr; itr; ++itr)
		{
			if (itr->GetWorld() != GetWorld())
				continue;

			if (itr->GetOwner() != nullptr)
			{
				m_hapticListener = *itr;
				break;
			}
		}
	}

	if (m_hapticListener == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] FCyberithPlugin : No VirtHapticListener-Object set in VirtHapticEmitter@'%s'."), *GetOwner()->GetName());
		return;
	}
			
	if (m_bAutoStartPlaying)
		Play();
}

void UVirtHapticEmitterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Stop();
}

// Called every frame
void UVirtHapticEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_bIsPlaying)
		return;

	if (!m_bLoop)
	{
		float timeDelta = (UGameplayStatics::GetUnpausedTimeSeconds(this) - m_timeStart);

		if (timeDelta > GetDuration())
		{
			Stop();
		}
	}
}

#if WITH_EDITOR

#include "Runtime/Engine/Public/SceneManagement.h"

void FVirtHapticEmitterVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	//cast the component into the expected component type
	if (const UVirtHapticEmitterComponent* emitter = Cast<const UVirtHapticEmitterComponent>(Component))
	{
		FColor color(255, 140, 0);
		DrawCircle(PDI, emitter->GetComponentLocation(), FVector::RightVector, FVector::ForwardVector, color, emitter->GetRange(), 64, 1, 2);
	}
}
#endif

