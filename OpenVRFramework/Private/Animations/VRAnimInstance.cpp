// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/VRAnimInstance.h"

#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Net/UnrealNetwork.h"

void UVRAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVRAnimInstance, HeadZLocation);
	DOREPLIFETIME(UVRAnimInstance, LeftHandPosition);
	DOREPLIFETIME(UVRAnimInstance, RightHandPosition);
	DOREPLIFETIME(UVRAnimInstance, LeftHandRotation);
	DOREPLIFETIME(UVRAnimInstance, RightHandRotation);
	DOREPLIFETIME(UVRAnimInstance, HeadRotation);

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
			HeadZLocation = VRUnitBase->HeadZLocation;
			HeadRotation = VRUnitBase->HMDRotation;
			HeadLocation = VRUnitBase->HMDPosition;
			IsVirtualizerEnabled = VRUnitBase->EnableVirtualizer;
			LeftHandPosition = VRUnitBase->LeftHandLocation;
			RightHandPosition = VRUnitBase->RightHandLocation;
			
		
	


		/*
			if (LeftHandRotation.Yaw != VRUnitBase->LeftHandRotation.Yaw)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"),  VRUnitBase->RightHandRotation.Yaw);
				//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), RightHandRotation.Yaw);
				UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), VRUnitBase->LeftHandRotation.Yaw);
				if (GEngine)
				{
					FString DebugMessage = FString::Printf(TEXT("Pitch: %f // %f"), LeftHandRotation.Yaw, VRUnitBase->LeftHandRotation.Yaw);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
				}
			}*/
			LeftHandRotation = VRUnitBase->LeftHandRotation;
			RightHandRotation = VRUnitBase->RightHandRotation;

			Crouch = VRUnitBase->CrouchedNormedZ;
			Velocity = VRUnitBase->NormedVelocity;
			VRotation = VRUnitBase->VRotation;
			VRotationOffset = VRUnitBase->VRotationOffset;
			ActorLocation = VRUnitBase->GetActorLocation();
			ActorRotation = VRUnitBase->GetActorRotation();
		}
	}
	
}

