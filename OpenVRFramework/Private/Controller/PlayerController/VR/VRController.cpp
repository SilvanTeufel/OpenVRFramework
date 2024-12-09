// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerController/VR/VRController.h"
#include "Engine/Engine.h" // Needed for GEngine->AddOnScreenDebugMessage
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AVRController::AVRController()
{
	TargetActor = nullptr;
	PreviousTargetActor = nullptr;
}

void AVRController::BeginPlay()
{
	Super::BeginPlay();

	// Start the repeating timer to call LineTraceTimerCallback
	GetWorldTimerManager().SetTimer(LineTraceTimerHandle, this, &AVRController::LineTraceTimerCallback, LineTraceInterval, true);
}

void AVRController::LineTraceTimerCallback()
{
	PerformLineTrace();
}

void AVRController::PerformLineTrace()
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) { return; }

    AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
    if (!VRUnit || !VRUnit->Camera) { return; }

    FVector StartLocation = VRUnit->Camera->GetComponentLocation();
    FVector ForwardVector = VRUnit->Camera->GetForwardVector();

    // We'll create 10 spheres along the forward vector
    const int32 NumSpheres = 15;
    const float SphereRadius = 20.0f;
    const float SphereDistance = 50.0f; // Distance between spheres
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(VRUnit);
    Params.bTraceComplex = true;
    Params.bReturnPhysicalMaterial = false;

    FHitResult BestHit;
    bool bSomethingHit = false;

    // Define our sphere collision shape
    FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

    for (int32 i = 0; i < NumSpheres; i++)
    {
        // Calculate the center of this sphere along the forward vector
        FVector SphereCenter = StartLocation + (ForwardVector * SphereDistance * i);

        FHitResult LocalHit;
        bool bHit = GetWorld()->SweepSingleByChannel(
            LocalHit,
            SphereCenter,
            SphereCenter,  // Start and end the same, we are just checking an area
            FQuat::Identity,
            ECC_Visibility,
            SphereShape,
            Params
        );

#if WITH_EDITOR
        // Visualize the sphere
       // DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, bHit ? FColor::Red : FColor::Green, false, 2.0f);
#endif

        if (bHit && LocalHit.GetActor())
        {
            // Take the first valid hit we find
            BestHit = LocalHit;
            bSomethingHit = true;
            break; // Stop checking further spheres
        }
    }

    // Process results
    if (bSomethingHit && BestHit.GetActor())
    {
        AActor* HitActor = BestHit.GetActor();
        if (HitActor != TargetActor)
        {
            // Remove highlight from previous target if needed
            if (TargetActor)
            {
                RemoveHighlightFromActor(TargetActor);
            }

            PreviousTargetActor = TargetActor;
            TargetActor = HitActor;
            HighlightActor(TargetActor, FLinearColor::Green);
        }
    }
    else
    {
        // No hit found
        if (TargetActor)
        {
            RemoveHighlightFromActor(TargetActor);
            TargetActor = nullptr;
        }
    }
}

void AVRController::HighlightActor(AActor* ActorToHighlight, FLinearColor OutlineColor)
{

    if (!ActorToHighlight) return;

    AUnitBase* UActor = Cast<AUnitBase>(ActorToHighlight);
    AProjectile* PActor = Cast<AProjectile>(ActorToHighlight);
    AMeleeWeapon* WActor = Cast<AMeleeWeapon>(ActorToHighlight);
    
    if (!UActor && !PActor && !WActor && !ActorToHighlight->ActorHasTag(FName("Highlightable"))) return;
    // If we already have a highlight light, do not create another one
    if (HighlightLight)
    {
        return;
    }

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
    if (!VRUnit || !VRUnit->Camera) return;

    FVector PlayerLocation = VRUnit->Camera->GetComponentLocation();
    FVector ActorLocation = ActorToHighlight->GetActorLocation();

    // Calculate midpoint between player and actor
    FVector Midpoint = (PlayerLocation + ActorLocation) * 0.5f;

    // Get the actor's bounding box to determine size
    FBox ActorBox = ActorToHighlight->GetComponentsBoundingBox(true);
    FVector Extent = ActorBox.GetExtent();
    float MaxDimension = Extent.GetMax(); 
    // MaxDimension gives an idea of how large the actor is. 
    // For example, a bigger actor might need a larger radius to fully illuminate.

    // You can use MaxDimension to scale properties. For example:
    float BaseAttenuation = 5000.f;
    float ScaledAttenuation = BaseAttenuation + (MaxDimension * 1.0f); // Increase radius based on size

    float BaseIntensity = 10000.0f;
    float ScaledIntensity = BaseIntensity + (MaxDimension * 50.0f); // Brighter light for bigger objects

    // Create a new Spot Light component
    HighlightLight = NewObject<USpotLightComponent>(ActorToHighlight);
    if (HighlightLight)
    {
        HighlightLight->RegisterComponent();

        // Set up the spotlight properties based on the actor's size
        HighlightLight->SetIntensity(ScaledIntensity);
        HighlightLight->SetLightColor(FLinearColor::Red);
        HighlightLight->SetAttenuationRadius(ScaledAttenuation);

        // Adjust the cone angles if desired, possibly based on size too
        HighlightLight->SetInnerConeAngle(15.f);
        HighlightLight->SetOuterConeAngle(30.f);

        // Attach the light to the actor
        HighlightLight->AttachToComponent(ActorToHighlight->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

        // Move the light to the midpoint
        HighlightLight->SetWorldLocation(Midpoint);

        // Rotate the light to point towards the actor
        FVector DirectionToActor = ActorLocation - Midpoint;
        FRotator LightRotation = DirectionToActor.Rotation();
        HighlightLight->SetWorldRotation(LightRotation);
    }

    UE_LOG(LogTemp, Log, TEXT("Highlighting Actor with spotlight: %s, Size: %.2f"), *ActorToHighlight->GetName(), MaxDimension);
}
void AVRController::RemoveHighlightFromActor(AActor* ActorToRemove)
{
	if (HighlightLight)
	{
		HighlightLight->DestroyComponent();
		HighlightLight = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Removed highlight spotlight from Actor: %s"), *ActorToRemove->GetName());
	}
}

void AVRController::GrabProjectile()
{
    if (!TargetActor) return;

    // Check if TargetActor is AProjectile
    AProjectile* Projectile = Cast<AProjectile>(TargetActor);
    if (!Projectile) 
    {
        // TargetActor is not a projectile
        return;
    }

    // Get the current pawn and cast to VRUnitBase to access RightMotionController
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;
    
    AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
    if (!VRUnit || !VRUnit->RightMotionController) return;

    // Get the velocity from the RightMotionController
    FVector ControllerVelocity = VRUnit->RightMotionController->GetComponentVelocity();

   // if (Projectile->MovementSpeed < ControllerVelocity.Size())
    Projectile->InitForGrab(FVector(10.f,0.f,0.f), 20.f);
    

    UE_LOG(LogTemp, Log, TEXT("SET SPEED AND GRAB!!"));
}