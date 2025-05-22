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
				// 2) mesh‐local ← world
				FTransform MCtoWorld =VRUnitBase->RightMotionController->GetComponentTransform();

				FRotator MCRExtraEuler(  +1*VRotation );  
				FTransform RTweak( MCRExtraEuler.Quaternion(), FVector::ZeroVector );
				MCtoWorld = RTweak*MCtoWorld;

				// 3) controller → world → mesh‐local
				FTransform DesiredBoneLocal = MeshWorldToLocal * MCtoWorld;

				// 4) extract rotation and log it
				FQuat RawQuat = DesiredBoneLocal.GetRotation();


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
					FTransform MCtoWorldL = VRUnitBase->LeftMotionController->GetComponentTransform();
					FRotator MCLExtraEuler(  -1*VRotation );  
					FTransform LTweak( MCLExtraEuler.Quaternion(), FVector::ZeroVector );
					MCtoWorldL = LTweak*MCtoWorldL;

					//UE_LOG(LogTemp, Warning, TEXT("VRUnitBase->CalibLeftOffset: %s"), *VRUnitBase->CalibLeftOffset.ToString());
					//FRotator OffExtraEuler(  -1*VRUnitBase->CalibLeftOffset );  
					//FTransform OffTweak( OffExtraEuler.Quaternion(), FVector::ZeroVector );
					//MCtoWorldL = OffTweak*MCtoWorldL;
					
					FTransform DesiredL = GetSkelMeshComponent()->GetComponentTransform()*MCtoWorldL;
					FQuat RawL         = DesiredL.GetRotation();
					
					LeftHandTargetTM.SetRotation( RawL ); // *FlipQuatL

					FRotator ExtraEuler( 0.f, -260.f, -260.f ); // WIth only Raw
					//FRotator ExtraEuler( 0.f, -290.f, -60.f );  //Without Offset Tweak
					//FRotator ExtraEuler( 60.f, -290.f, -60.f );  //With Offset Tweak
					FTransform Tweak( ExtraEuler.Quaternion(), FVector::ZeroVector );
					LeftHandTargetTM = Tweak * LeftHandTargetTM;
				
					LeftHandRotation = LeftHandTargetTM.Rotator();
				
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

