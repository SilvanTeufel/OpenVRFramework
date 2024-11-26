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
		
	


		
			if (RightHandRotation.Yaw != VRUnitBase->RightHandRotation.Yaw)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"),  VRUnitBase->RightHandRotation.Yaw);
				//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), RightHandRotation.Yaw);
				UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), VRUnitBase->RightHandRotation.Yaw);
				if (GEngine)
				{
					FString DebugMessage = FString::Printf(TEXT("Yaw: %f // %f"), RightHandRotation.Yaw, VRUnitBase->RightHandRotation.Yaw);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
				}
			}
			//RightHandRotation.Yaw = VRUnitBase->RightHandRotation.Yaw;
			//RightHandRotation.Roll = FMath::Clamp(VRUnitBase->RightHandRotation.Roll, -140.f, -120.f);
			RightHandRotation.Roll = VRUnitBase->RightHandRotation.Roll;
			//RightHandRotation.Yaw = VRUnitBase->RightHandRotation.Yaw;
		
			// Clamp the normalized value between 0 and 1
			RightHandRotation.Yaw = VRUnitBase->RightHandRotation.Yaw;

			// Now you can use NormalizedYaw in your application
		
	
			RightHandRotation.Pitch = VRUnitBase->RightHandRotation.Pitch;

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

