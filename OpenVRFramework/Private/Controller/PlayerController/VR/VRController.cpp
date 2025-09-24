// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerController/VR/VRController.h"
#include "Engine/Engine.h" // Needed for GEngine->AddOnScreenDebugMessage
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Characters/Unit/VRUnit/VRUnitBase.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
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

void AVRController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    HandleGrabbedProjectile();
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
    const int32 NumSpheres = 16;
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
        //FVector HandLocation = FVector(VRUnit->RightHandLocation.X, VRUnit->RightHandLocation.Y, VRUnit->RightHandLocation.Z*0.2f);
        FVector SphereCenter = StartLocation + (ForwardVector * SphereDistance * i);// + HandLocation*i/NumSpheres;
        
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
        if (i == 6) 
            DrawDebugSphere(GetWorld(), SphereCenter, 1.f, 12, bHit ? FColor::Red : FColor::Green, false, 2.0f);

        //DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, bHit ? FColor::Red : FColor::Green, false, 2.0f);
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


  //  AddOrUpdateCrosshairLight(MidSphereCenter, true);
   // AddOrUpdateCrosshair(MidSphereCenter, true);
}

void AVRController::AddOrUpdateCrosshair(const FVector& Location, bool bShouldBeVisible)
{
    if (!CrosshairBillboard)
    {
        APawn* MyPawn = GetPawn();
        if (MyPawn)
        {
            AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
            if (VRUnit && VRUnit->Camera)
            {
                CrosshairBillboard = NewObject<UBillboardComponent>(VRUnit->Camera);
                if (CrosshairBillboard)
                {
                    CrosshairBillboard->RegisterComponent();
                    CrosshairBillboard->AttachToComponent(VRUnit->Camera, FAttachmentTransformRules::KeepRelativeTransform);

                    CrosshairBillboard->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
                    CrosshairBillboard->SetVisibility(bShouldBeVisible);
                    // Optional: Set a sprite for the billboard, assuming CrosshairTexture is a valid UTexture2D*
                    // CrosshairBillboard->SetSprite(CrosshairTexture);
                }
            }
        }
    }
}

void AVRController::AddOrUpdateCrosshairLight(const FVector& Location, bool bShouldBeVisible)
{
    // Erstelle das Punktlicht, falls es noch nicht existiert
    if (!CrosshairLight)
    {
        APawn* MyPawn = GetPawn();
        if (MyPawn)
        {
            AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
            if (VRUnit && VRUnit->Camera)
            {
                // Erzeuge das Punktlicht als Kind der Kamera
                CrosshairLight = NewObject<UPointLightComponent>(VRUnit->Camera);
                if (CrosshairLight)
                {
                    CrosshairLight->RegisterComponent();
                    CrosshairLight->SetIntensity(50000.f);
                    CrosshairLight->SetLightColor(FLinearColor::Blue);
                    CrosshairLight->SetAttenuationRadius(20.f);
                    CrosshairLight->SetCastShadows(false);

                    // Licht an die Kamera anhängen, so dass es im Raum positioniert wird
                    CrosshairLight->AttachToComponent(VRUnit->Camera, FAttachmentTransformRules::KeepRelativeTransform);
                    CrosshairLight->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
                }
            }
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

FVector InitialHandLocation;
FVector InitialProjectileLocation;

// File-scope helpers to improve grab/ungrab robustness:
// - GrabInitialTargetActor stores the target seen at the moment of Grab so Ungrab can still use it
// - LastGrabToggleTime + GrabToggleCooldown provide a small debounce to avoid spurious double-toggles
static AActor* GrabInitialTargetActor = nullptr;
static float LastGrabToggleTime = 0.f;
static const float GrabToggleCooldown = 0.15f; // 150ms debounce

void AVRController::GrabProjectile(AVRUnitBase* VRUnit)
{
    if (!VRUnit || !VRUnit->RightMotionController) return;

    // Very small debounce to avoid accidental double toggles when input events are noisy
    if (GetWorld())
    {
        float Now = GetWorld()->GetTimeSeconds();
        if (Now - LastGrabToggleTime < GrabToggleCooldown)
            return;
        LastGrabToggleTime = Now;
    }

    // Wenn bereits eines gegriffen ist, dann delegiere die Freigabe an UnGrabProjectile
    if (GrabbedProjectile != nullptr)
    {
        UnGrabProjectile(VRUnit);
        return;
    }

    if (!TargetActor) return;

    AProjectile* Projectile = Cast<AProjectile>(TargetActor);
    if (!Projectile) return;

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    // Store which projectile is grabbed
    GrabbedProjectile = Projectile;

    // Store initial positions at the moment of grabbing
    InitialHandLocation = VRUnit->RightHandLocation;
    InitialProjectileLocation = Projectile->GetActorLocation();

    // Also save the currently targeted actor so we have a fallback when releasing
    GrabInitialTargetActor = TargetActor;

    FVector Direction = (VRUnit->GetActorLocation() - Projectile->GetActorLocation()).GetSafeNormal() * 10.f;

    // Initialize or configure your projectile as needed; defensive: ensure projectile disables physics/collision while grabbed
    Projectile->InitForGrab(Direction, 20.f);

    UE_LOG(LogTemp, Log, TEXT("Projectile grabbed!"));
}


void AVRController::UnGrabProjectile(AVRUnitBase* VRUnit)
{
    if (!VRUnit || !VRUnit->RightMotionController) return;

    if (GrabbedProjectile != nullptr)
    {
        // Defensive log
        UE_LOG(LogTemp, Log, TEXT("UnGrabbing projectile."));

        // If the grabbed projectile pointer is no longer valid (destroyed remotely etc.), clear and bail out.
        if (!IsValid(GrabbedProjectile))
        {
            UE_LOG(LogTemp, Warning, TEXT("GrabbedProjectile invalid or pending kill during UnGrab; clearing local reference."));
            GrabbedProjectile = nullptr;
            GrabInitialTargetActor = nullptr;
            return;
        }

        // Use the currently targeted actor if present, otherwise fall back to the actor that was targeted when grabbing began.
        AActor* ReleaseTarget = TargetActor ? TargetActor : GrabInitialTargetActor;

        if (ReleaseTarget)
        {
            // Lass das Projektil zum aktuell anvisierten Target fliegen oder zum beim Grab gespeicherten Ziel
            // InitForUnGrab setzt nun automatisch die MovementSpeed auf das Projectile-Property UnGrabMovementSpeed
            GrabbedProjectile->InitForUnGrab(ReleaseTarget, VRUnit, GrabbedProjectile->GetActorLocation());
            // InitForUnGrab stellt FollowTarget bereits korrekt ein; keine doppelte Zuweisung hier.
        }
        else
        {
            // Falls kein Target vorhanden: einfach loslassen, aber FollowTarget deaktiviert
            GrabbedProjectile->InitForUnGrab(nullptr, VRUnit, GrabbedProjectile->GetActorLocation());
            // InitForUnGrab wird FollowTarget auf false setzen, daher hier keine zusätzliche Zuweisung nötig.
        }

        // Clear saved target so it won't be reused accidentally
        GrabInitialTargetActor = nullptr;

        // Release the reference (client controller no longer manipulates the projectile)
        GrabbedProjectile = nullptr;
    }
}

void AVRController::HandleGrabbedProjectile()
{
    // Ensure grabbed projectile pointer is valid and hasn't been destroyed
    if (!GrabbedProjectile) return;
    if (!IsValid(GrabbedProjectile))
    {
        UE_LOG(LogTemp, Warning, TEXT("GrabbedProjectile became invalid during HandleGrabbedProjectile; clearing reference."));
        GrabbedProjectile = nullptr;
        GrabInitialTargetActor = nullptr;
        return;
    }

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    AVRUnitBase* VRUnit = Cast<AVRUnitBase>(MyPawn);
    if (!VRUnit || !VRUnit->RightMotionController) return;

    // Calculate the offset from the initial hand position
    FVector HandOffset = VRUnit->RightHandLocation - InitialHandLocation;

    // Apply offset to the projectile's initial position without pulling toward the player
    FVector NewLocation = InitialProjectileLocation + HandOffset * GrabbedProjectileHandSensitivity;

    // Teleport the actor to the new location without generating sweeps/overlaps to avoid immediate OnOverlap destruction.
    GrabbedProjectile->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
}