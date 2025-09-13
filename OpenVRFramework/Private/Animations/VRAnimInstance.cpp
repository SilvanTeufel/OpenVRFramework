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
			
			ForwardDirection.Normalize();
			HeadLocation = VRUnitBase->HMDPosition;
			HeadLocation.X -= VRUnitBase->OriginCalibrationOffset.X;
			HeadLocation.Y -= HeadLocation.Y-VRUnitBase->OriginCalibrationOffset.Y;
			HeadLocation += ForwardDirection*2.f;
			
			WorldHeadLocation.Z = VRUnitBase->HMDPosition.Z + VRUnitBase->GetActorLocation().Z;

			WorldHeadLocation.X = (VRUnitBase->HMDPosition.X-VRUnitBase->OriginCalibrationOffset.X)*2.f + VRUnitBase->GetActorLocation().X;
			WorldHeadLocation.Y = (VRUnitBase->HMDPosition.Y-VRUnitBase->OriginCalibrationOffset.Y)*2.f + VRUnitBase->GetActorLocation().Y;

			IsVirtualizerEnabled = VRUnitBase->EnableVirtualizer;
			LeftHandPosition = VRUnitBase->LeftHandLocation;
			RightHandPosition = VRUnitBase->RightHandLocation;

			
			const FTransform MeshWorldToLocal = GetSkelMeshComponent()->GetComponentTransform().Inverse();
			
			if (VRUnitBase->RightMotionController)
			{
				// 2) mesh‐local ← world
				FTransform MCtoWorld =VRUnitBase->RightMotionController->GetComponentTransform();
				if (VRUnitBase->EnableVirtualizer)
				{
					FRotator MCRExtraEuler(  +1*VRotation );
					FTransform RTweak( MCRExtraEuler.Quaternion(), FVector::ZeroVector );
					MCtoWorld = RTweak*MCtoWorld;
				}else
				{
					FRotator MCRExtraEuler(  FRotator(0.f, +1*VRUnitBase->HMDRotation.Yaw, 0.f) );
					FTransform RTweak( MCRExtraEuler.Quaternion(), FVector::ZeroVector );
					MCtoWorld = RTweak*MCtoWorld;
				}

				// 3) controller → world → mesh‐local
				FTransform DesiredBoneLocal = MeshWorldToLocal * MCtoWorld;

				// 4) extract rotation and log it
				FQuat RawQuat = DesiredBoneLocal.GetRotation();


				// 2) Define your “flip” (tweak these values as needed)
				static const FRotator FlipRotator( /*Pitch*/ -220.f, /*Yaw*/ 260.f, /*Roll*/-260.f );
				FQuat FlipQuat = FlipRotator.Quaternion();
				FQuat CandidateB = RawQuat * FlipQuat;   // apply flip last
			
				RightHandTargetTM.SetRotation(CandidateB); 

				//FRotator ExtraEuler( 40.f, -20.f, 0.f ); // WIth only Raw
				FRotator ExtraEuler( 40.f, 10.f, 40.f ); // WIth only Raw
				FTransform Tweak( ExtraEuler.Quaternion(), FVector::ZeroVector );
				RightHandTargetTM = Tweak * RightHandTargetTM;
			
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
					if (VRUnitBase->EnableVirtualizer)
					{
						FRotator MCLExtraEuler(  -1*VRotation );  
						FTransform LTweak( MCLExtraEuler.Quaternion(), FVector::ZeroVector );
						MCtoWorldL = LTweak*MCtoWorldL;
					}else
					{
						FRotator MCLExtraEuler(  FRotator(0.f, -1*VRUnitBase->HMDRotation.Yaw, 0.f) );  
						FTransform LTweak( MCLExtraEuler.Quaternion(), FVector::ZeroVector );
						MCtoWorldL = LTweak*MCtoWorldL;
					}
					
					FTransform DesiredL = GetSkelMeshComponent()->GetComponentTransform()*MCtoWorldL;
					FQuat RawL         = DesiredL.GetRotation();
					
					LeftHandTargetTM.SetRotation( RawL );
					
					FRotator ExtraEuler( -40.f, -220.f, -240.f ); // WIth only Raw
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

			Crouch = VRUnitBase->CrouchedNormedZ;

			Velocity = VRUnitBase->NormedVelocity;
			VRotation = VRUnitBase->VRotation;
			VRotationOffset = VRUnitBase->VRotationOffset;
			ActorLocation = VRUnitBase->GetActorLocation();
			ActorRotation = VRUnitBase->GetActorRotation();
		}
	}
	
}

