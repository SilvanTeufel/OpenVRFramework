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
			LeftHandRotation = VRUnitBase->LeftHandRotation;
		
	


		
			if (RightHandRotation.Roll != VRUnitBase->RightHandRotation.Roll)
			{
				UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"),  VRUnitBase->RightHandRotation.Yaw);
				//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), RightHandRotation.Yaw);
				UE_LOG(LogTemp, Warning, TEXT("Roll: %f"), RightHandRotation.Roll);
			}
			//RightHandRotation.Yaw = VRUnitBase->RightHandRotation.Yaw;
			RightHandRotation.Roll = FMath::Clamp(VRUnitBase->RightHandRotation.Roll, -140.f, -120.f);
			RightHandRotation.Yaw = FMath::Clamp(VRUnitBase->RightHandRotation.Yaw, 70.f, 130.f);
			if (RightHandRotation.Roll <= 0.f)
			{
				RightHandRotation.Pitch = 70.f + (-1)*(VRUnitBase->RightHandRotation.Pitch-70.f);
	
				//RightHandRotation.Yaw = FMath::Clamp(70.f + (-1)*(VRUnitBase->RightHandRotation.Yaw-70.f), 70.f, 130.f);
				
			}
			else
			{
				RightHandRotation.Pitch = VRUnitBase->RightHandRotation.Pitch;
				//RightHandRotation.Yaw = FMath::Clamp(VRUnitBase->RightHandRotation.Yaw, 70.f, 130.f);
			}
			
			//if (RightHandRotation.Pitch <= -360.f) RightHandRotation.Pitch = RightHandRotation.Pitch*(-1.f);
			//UE_LOG(LogTemp, Warning, TEXT("Roll: %f"), RightHandRotation.Roll);
			//UE_LOG(LogTemp, Warning, TEXT("Pitch: %f"), RightHandRotation.Pitch);
			//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), RightHandRotation.Yaw);
			//if (RightHandRotation.Pitch < 45.f) RightHandRotation.Pitch = RightHandRotation.Pitch*(-1.f);
			//RightHandRotation.Pitch = RightHandRotation.Pitch-45.f;
			//UE_LOG(LogTemp, Warning, TEXT("2 RightHandRotation.Pitch: %f"), RightHandRotation.Pitch);
			Crouch = VRUnitBase->CrouchedNormedZ;
			Velocity = VRUnitBase->NormedVelocity;
			VRotation = VRUnitBase->VRotation;
			VRotationOffset = VRUnitBase->VRotationOffset;
			ActorLocation = VRUnitBase->GetActorLocation();
			ActorRotation = VRUnitBase->GetActorRotation();
		}
	}
	
}

