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
	DOREPLIFETIME(UVRAnimInstance, WorldHeadLocation);
}

static FRotator MapRightHandRotationToFabrik(const FRotator& Input, float Diff)
{
	float baseP = Input.Pitch;
	float baseY = Input.Yaw;
	float outR = Input.Roll;

	float outP, outY;


	if (baseY <= 0.f)
	{
		outP = baseP * -1.5f - 90.f;
	}
	else
	{
		outP = baseP *  1.5f + 45.f;
	}


	{
		outY = FMath::Abs(baseY) *  0.75f + 30.f;
	}

	if (Diff > 50.f)
	{
		outP = 120.f;
		outY = 90.f;
	}

	return FRotator(outP, outY, outR);
}

static FRotator MapRLeftHandRotationToFabrik(const FRotator& Input, float Diff)
{
	float baseP = Input.Pitch;
	float baseY = Input.Yaw;
	float baseR = Input.Roll;

	float outP, outY;

	
	if (baseR <= 0.f)
	{
		outP = baseP * 1.5f -135.f;
	}
	else
	{
		outP = baseP *  -1.5f + 90.f;
	}


	{
		outY = FMath::Abs(baseY) *  -0.75f - 30.f;
	}
	
	if (Diff > 50.f)
	{
		outP = -30.f;
		outY = -90.f;
	}
	
	return FRotator(outP, outY, baseR);
}

void UVRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AActor* OwningActor = GetOwningActor();

	if (OwningActor != nullptr)
	{
		AVRUnitBase* VRUnitBase = Cast<AVRUnitBase>(OwningActor);

		if(VRUnitBase != nullptr)
		{
			HeadZLocation = VRUnitBase->HeadZLocation;
			HeadRotation = VRUnitBase->HMDRotation;
			HeadLocation = VRUnitBase->HMDPosition;
			WorldHeadLocation = VRUnitBase->HMDPosition + VRUnitBase->GetActorLocation();
			WorldHeadLocation.X = VRUnitBase->HMDPosition.X*1.5 + VRUnitBase->GetActorLocation().X;
			IsVirtualizerEnabled = VRUnitBase->EnableVirtualizer;
			LeftHandPosition = VRUnitBase->LeftHandLocation;
			RightHandPosition = VRUnitBase->RightHandLocation;

		
			/*
			if (GEngine)
			{
				// Print X, Y and Z in one go
				FString DebugMessage = FString::Printf(
					TEXT("HeadLocation: X = %f, Y = %f, Z = %f"),
					HeadLocation.X,
					HeadLocation.Y,
					HeadLocation.Z
				);
				GEngine->AddOnScreenDebugMessage(
					 -1,
					 5.0f,
					 FColor::Green,
					DebugMessage
				);
			}
		
			if (GEngine)
			{
				FString DebugMessage3 = FString::Printf(TEXT("Pitch // Roll // Yaw: %f // %f // %f "), VRUnitBase->LeftHandRotation.Pitch, VRUnitBase->LeftHandRotation.Roll, VRUnitBase->LeftHandRotation.Yaw);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage3);
			}*/
			float RightDiff;
			float LeftDiff;
			if (!VRUnitBase->EnableVirtualizer)
			{
				RightDiff = HeadZLocation-RightHandPosition.Z;
				LeftDiff = HeadZLocation-LeftHandPosition.Z;
			}else
			{
				RightDiff = HeadZLocation-RightHandPosition.Z+25.f;
				LeftDiff = HeadZLocation-LeftHandPosition.Z+25.f;
			}
			
			RightHandRotation = MapRightHandRotationToFabrik(VRUnitBase->RightHandRotation, RightDiff);
			
			
			// separately on Pitch, Yaw, Roll:
			SmoothedRightHandRot = FMath::RInterpTo(
					SmoothedRightHandRot,       // from (last frame)
					RightHandRotation,          // to   (new target)
					DeltaSeconds,               // how much time has passed
					RotationInterpSpeed         // your speed scalar
				);

			LeftHandRotation = MapRLeftHandRotationToFabrik(VRUnitBase->LeftHandRotation, LeftDiff);

			SmoothedLeftHandRot = FMath::RInterpTo(
					SmoothedLeftHandRot,       // from (last frame)
					LeftHandRotation,          // to   (new target)
					DeltaSeconds,               // how much time has passed
					RotationInterpSpeed         // your speed scalar
				);
			/*
			        UE_LOG(LogTemp, Warning, TEXT(
                        "Right Hand Rotation -> Roll: %6.2f | Pitch: %6.2f | Yaw: %6.2f"
                    ), RightHandRotation.Roll, RightHandRotation.Pitch, RightHandRotation.Yaw);
			*/
			Crouch = VRUnitBase->CrouchedNormedZ;
			/*
			if (GEngine)
			{
				FString DebugMessage3 = FString::Printf(TEXT("Crouch %f "), Crouch);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage3);
			}*/
			Velocity = VRUnitBase->NormedVelocity;
			VRotation = VRUnitBase->VRotation;
			VRotationOffset = VRUnitBase->VRotationOffset;
			ActorLocation = VRUnitBase->GetActorLocation();
			ActorRotation = VRUnitBase->GetActorRotation();
		}
	}
	
}

