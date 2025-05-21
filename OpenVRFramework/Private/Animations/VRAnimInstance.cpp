// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/VRAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
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
			FVector ForwardDirection = FRotationMatrix(VRUnitBase->HMDRotation).GetScaledAxis(EAxis::X);

			// Normalize the movement direction to ensure consistent movement speed
			ForwardDirection.Normalize();
			HeadLocation = VRUnitBase->HMDPosition;
			HeadLocation.X -= VRUnitBase->OriginCalibrationOffset.X;
			HeadLocation.Y -= HeadLocation.Y-VRUnitBase->OriginCalibrationOffset.Y;
			HeadLocation += ForwardDirection*5.f;
			WorldHeadLocation.Z = VRUnitBase->HMDPosition.Z + VRUnitBase->GetActorLocation().Z;
			WorldHeadLocation.X = (VRUnitBase->HMDPosition.X-VRUnitBase->OriginCalibrationOffset.X)*3.f + VRUnitBase->GetActorLocation().X;
			WorldHeadLocation.Y = (VRUnitBase->HMDPosition.Y-VRUnitBase->OriginCalibrationOffset.Y)*2.75f + VRUnitBase->GetActorLocation().Y;;
			WorldHeadLocation += ForwardDirection*5.f;
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
			*/

			//RightHandRotation = VRUnitBase->RightHandRotation;

			// 2) mesh‐local ← world
			const FTransform MeshWorldToLocal = GetSkelMeshComponent()->GetComponentTransform().Inverse();

			
			if (VRUnitBase->RightMotionController)
			{
				const FTransform MCtoWorld = VRUnitBase->RightMotionController->GetComponentTransform();

				// 2) mesh‐local ← world
				//const FTransform MeshWorldToLocal = GetSkelMeshComponent()->GetComponentTransform().Inverse();

				// 3) controller → world → mesh‐local
				FTransform DesiredBoneLocal = MeshWorldToLocal * MCtoWorld;

				// 4) extract rotation and log it
				FQuat RawQuat = DesiredBoneLocal.GetRotation();
				UE_LOG(LogTemp, Warning,
					TEXT("RawHand Rot: P=%5.1f  Y=%5.1f  R=%5.1f"),
					RawQuat.Rotator().Pitch,
					RawQuat.Rotator().Yaw,
					RawQuat.Rotator().Roll);

				// 2) Define your “flip” (tweak these values as needed)
				static const FRotator FlipRotator( /*Pitch*/ -220.f, /*Yaw*/ 260.f, /*Roll*/-260.f );
				FQuat FlipQuat = FlipRotator.Quaternion();
				FQuat CandidateB = RawQuat * FlipQuat;   // apply flip last
			
				RightHandTargetTM.SetRotation(CandidateB); 

			
			
				RightHandRotation = RightHandTargetTM.Rotator();//MapRightHandRotationToFabrik(VRUnitBase->RightHandRotation, RightDiff);
			
			
				// separately on Pitch, Yaw, Roll:
				SmoothedRightHandRot = FMath::RInterpTo(
						SmoothedRightHandRot,       // from (last frame)
						RightHandRotation,          // to   (new target)
						DeltaSeconds,               // how much time has passed
						RotationInterpSpeed         // your speed scalar
					);
			}


			if (VRUnitBase->LeftMotionController)
			{
				const FTransform MCtoWorldL = VRUnitBase->LeftMotionController->GetComponentTransform();
				FTransform DesiredL = GetSkelMeshComponent()->GetComponentTransform()*MCtoWorldL;
				FQuat RawL         = DesiredL.GetRotation();

				// the left bone is often mirrored in yaw relative to the right,
				// so you can usually reuse your right‐hand Flip but swap the sign on Yaw:
				static const FRotator FlipL(  /*Pitch*/ -220.f, /*Yaw*/ 260.f, /*Roll*/-260.f );
				FQuat FlipQuatL = FlipL.Quaternion();

				// try both orders if you’re not 100% sure:
				// FQuat CandidateA = FlipQuatL * RawL;
				// FQuat CandidateB = RawL * FlipQuatL;
				// LeftHandTargetTM.SetRotation(CandidateA);
				LeftHandTargetTM.SetRotation( RawL*FlipQuatL ); // *FlipQuatL
			
				LeftHandRotation = LeftHandTargetTM.Rotator()*-1;
				//LeftHandRotation.Roll = LeftHandTargetTM.Rotator().Roll-200.f;//MapRLeftHandRotationToFabrik(VRUnitBase->LeftHandRotation, LeftDiff);
				//LeftHandRotation.Pitch = LeftHandTargetTM.Rotator().Pitch+0.f;
				LeftHandRotation.Yaw = LeftHandTargetTM.Rotator().Yaw-180.f;
	
				
				SmoothedLeftHandRot = FMath::RInterpTo(
						SmoothedLeftHandRot,       // from (last frame)
						LeftHandRotation,          // to   (new target)
						DeltaSeconds,               // how much time has passed
						RotationInterpSpeed         // your speed scalar
					);
			}
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

