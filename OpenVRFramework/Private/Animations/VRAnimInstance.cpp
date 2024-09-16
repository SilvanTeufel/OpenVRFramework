// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/VRAnimInstance.h"

#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Net/UnrealNetwork.h"

void UVRAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVRAnimInstance, Head);
	DOREPLIFETIME(UVRAnimInstance, LeftHand);
	DOREPLIFETIME(UVRAnimInstance, RightHand);
}

void UVRAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);

	AActor* OwningActor = GetOwningActor();

	if (OwningActor != nullptr)
	{
		AVRUnitBase* VRUnitBase = Cast<AVRUnitBase>(OwningActor);

		if(VRUnitBase != nullptr)
		{
			Head = VRUnitBase->HeadLocation;
			LeftHand = VRUnitBase->LeftHandLocation;
			RightHand = VRUnitBase->RightHandLocation;
			LeftHandRotation = VRUnitBase->LeftHandRotation;
			RightHandRotation = VRUnitBase->RightHandRotation;
			Knee = VRUnitBase->NormalizedZ;
			Velocity = 1.f;
		}
	}
	
}

