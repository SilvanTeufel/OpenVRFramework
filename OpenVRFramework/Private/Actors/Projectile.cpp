// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Controller/AIController/UnitControllerBase.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/UnitBaseHealthBar.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = TickInterval; 
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	// Use query-only collision and ensure overlap events are generated.
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	Mesh->SetGenerateOverlapEvents(true);

	// Defensive: Ensure no physics simulation or rigid-body collisions happen.
	// This prevents unexpected physical interactions while keeping query/overlap functionality.
	if (Mesh)
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetNotifyRigidBodyCollision(false);
		// Make sure the object type is a dynamic world object and explicitly set responses.
		Mesh->SetCollisionObjectType(ECC_WorldDynamic);
		// Reset responses to be conservative: ignore everything then overlap pawns/world static so OnOverlap triggers.
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		// Wichtig: Sichtbarkeit blocken, damit LineTrace/Sweep das Projektil treffen kann
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	if (HasAuthority())
	{
		bReplicates = true;
		SetReplicateMovement(true);
	}
}

void AProjectile::Init(AActor* TargetActor, AActor* ShootingActor)
{
	Target = TargetActor;
	Shooter = ShootingActor;

	if(TargetLocation.IsZero())
		TargetLocation = TargetActor->GetActorLocation();

	
	if(ShootingActor)
		ShooterLocation = ShootingActor->GetActorLocation();
	
	
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Shooter);
	if(ShootingUnit)
	{
		Damage = ShootingUnit->Attributes->GetAttackDamage();
		TeamId = ShootingUnit->TeamId;
		MovementSpeed = ShootingUnit->Attributes->GetProjectileSpeed();

		if (ShootingUnit->IsVisibileEnemy || ShootingUnit->IsMyTeam)
		{

			SetVisibility(true);
		}
		else
		{

			SetVisibility(false);
		}
	}
}

void AProjectile::InitForAbility(AActor* TargetActor, AActor* ShootingActor)
{
	Target = TargetActor;
	Shooter = ShootingActor;
	GotGrabed = false;
	
	if(TargetLocation.IsZero())
		TargetLocation = TargetActor->GetActorLocation();

	
	if(ShootingActor)
		ShooterLocation = ShootingActor->GetActorLocation();
	
	
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Shooter);
	if(ShootingUnit)
	{
		Damage = ShootingUnit->Attributes->GetAttackDamage();
		TeamId = ShootingUnit->TeamId;

		if (ShootingUnit->IsVisibileEnemy || ShootingUnit->IsMyTeam)
		{

			SetVisibility(true);
		}
		else
		{

			SetVisibility(false);
		}
		
	}
	
}

void AProjectile::InitForUnGrab(AActor* TargetActor, AActor* ShootingActor, FVector StartLocation)
{
	Target = TargetActor;
	Shooter = ShootingActor;
	GotGrabed = false;
	
	// Ziel beim Loslassen immer neu setzen (falls kein Ziel übergeben, bleibt TargetLocation auf StartLocation)
	TargetLocation = TargetActor ? TargetActor->GetActorLocation() : StartLocation;

	// Startpunkt ist die aktuelle Projektilposition beim Loslassen
	ShooterLocation = StartLocation;

	// Wenn ein Target vorhanden ist, folge diesem; ansonsten nicht — das Projektil soll in Ruhe bleiben.
	if (TargetActor)
	{
		FollowTarget = true;
		// Verwende die im Projektil deklarierte UnGrab-MovementSpeed als Startgeschwindigkeit
		MovementSpeed = UnGrabMovementSpeed;
	}
	else
	{
		// Kein Ziel: das Projektil soll nicht weiterfliegen, sondern liegen bleiben
		FollowTarget = false;
		MovementSpeed = 0.f;
	}

	AUnitBase* ShootingUnit = Cast<AUnitBase>(Shooter);
	if(ShootingUnit)
	{
		Damage = ShootingUnit->Attributes->GetAttackDamage();
		TeamId = ShootingUnit->TeamId;

		if (ShootingUnit->IsVisibileEnemy || ShootingUnit->IsMyTeam)
		{
			SetVisibility(true);
		}
		else
		{
			SetVisibility(false);
		}
		
	}
	
	// Re-enable collision/overlap events when released so projectile can interact normally again.
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Mesh->SetGenerateOverlapEvents(true);

		// Keep physics off by default; projectile movement is driven manually in Tick when needed.
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetNotifyRigidBodyCollision(false);

		// Restore conservative collision object type and responses used in constructor
		Mesh->SetCollisionObjectType(ECC_WorldDynamic);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		// Sichtbarkeit blocken, damit die Zielerfassung nach dem Loslassen weiter funktioniert
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	// Reset LifeTime so the projectile won't be destroyed immediately after ungrab if MaxLifeTime is small
	LifeTime = 0.f;
}

void AProjectile::InitForGrab(FVector VelocityWhileGrabbed, float MLTime)
{
	// Set a fresh lifetime window for the grabbed state so it doesn't immediately expire
	MaxLifeTime = MLTime;
	GrabVelocity = VelocityWhileGrabbed;
	GotGrabed = true;

	// Reset accumulated life while grabbed so it won't be destroyed while player manipulates it
	LifeTime = 0.f;

	// While grabbed we do not want the projectile to auto-follow any target
	FollowTarget = false;

	// Ensure mesh is visible when grabbed (defensive)
	SetVisibility(true);

	// Defensive: While grabbed the projectile must not participate in physics collisions.
	// We disable collision and overlap events completely to avoid immediate OnOverlap-based destruction
	// while the player holds and moves the projectile.
	if (Mesh)
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetNotifyRigidBodyCollision(false);

		// Disable collision and overlap events while grabbed
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetGenerateOverlapEvents(false);

		// Still keep the configured responses so they are known when re-enabling later
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectile, Target);
	DOREPLIFETIME(AProjectile, Shooter);
	DOREPLIFETIME(AProjectile, Mesh);
	DOREPLIFETIME(AProjectile, Material);
	DOREPLIFETIME(AProjectile, Damage);
	DOREPLIFETIME(AProjectile, LifeTime);
	DOREPLIFETIME(AProjectile, MaxLifeTime);
	DOREPLIFETIME(AProjectile, TeamId);
	DOREPLIFETIME(AProjectile, MovementSpeed);
	// Repliziere die beim UnGrab verwendete Geschwindigkeit
	DOREPLIFETIME(AProjectile, UnGrabMovementSpeed);
	DOREPLIFETIME(AProjectile, DestructionDelayTime);
	DOREPLIFETIME(AProjectile, RotateMesh);
	DOREPLIFETIME(AProjectile, RotationSpeed);
	DOREPLIFETIME(AProjectile, TargetLocation);
	DOREPLIFETIME(AProjectile, ShooterLocation);
	DOREPLIFETIME(AProjectile, FollowTarget);
	DOREPLIFETIME(AProjectile, MaxPiercedTargets);
	DOREPLIFETIME(AProjectile, PiercedTargets);
	DOREPLIFETIME(AProjectile, IsHealing);
	DOREPLIFETIME(AProjectile, IsBouncingBack);
	DOREPLIFETIME(AProjectile, IsBouncingNext);
	DOREPLIFETIME(AProjectile, BouncedBack);
	DOREPLIFETIME(AProjectile, ProjectileEffect); // Added for Build
	DOREPLIFETIME(AProjectile, GotGrabed); // Added for Build
	DOREPLIFETIME(AProjectile, GrabVelocity); // Added for Build
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime += DeltaTime;

	if (GotGrabed)
	{
		//AddActorWorldOffset(GrabVelocity);
		return;
	}

	
	if(RotateMesh)
	{
		// Calculate rotation amount based on DeltaTime and RotationSpeed
	
		FRotator NewRotation = FRotator(RotationSpeed.X * DeltaTime, RotationSpeed.Y * DeltaTime, RotationSpeed.Z * DeltaTime);

		// Apply rotation to the mesh
		if (Mesh) // Assuming your mesh component is named MeshComponent
		{
			Mesh->AddLocalRotation(NewRotation);
		}
	}
	
	if(LifeTime > MaxLifeTime && FollowTarget)
	{
		Impact(Target);
		Destroy(true, false);
	}else if(LifeTime > MaxLifeTime)
	{
		Destroy(true, false);
	}else if(Target)
	{
		AUnitBase* TargetToAttack = Cast<AUnitBase>(Target);
		
		if(TargetToAttack && TargetToAttack->GetUnitState() != UnitData::Dead && FollowTarget) 
		{
			if(FollowTarget)
			{
				const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetToAttack->GetActorLocation());
				AddActorWorldOffset(Direction * MovementSpeed * DeltaTime);
			}
		}else if(FollowTarget)
		{
				Destroy(true, false);
		}else
		{
				const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ShooterLocation, TargetLocation);
            	AddActorWorldOffset(Direction * MovementSpeed * DeltaTime);
		}

		// Calculate the distance between the projectile and the target
		float DistanceToTarget = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if(DistanceToTarget <= MovementSpeed * DeltaTime && FollowTarget)
		{
			Impact(Target);
			Destroy(true, false);
		}
		
	}
	
	
}

void AProjectile::Impact_Implementation(AActor* ImpactTarget)
{
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Shooter);
	AUnitBase* UnitToHit = Cast<AUnitBase>(ImpactTarget);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile ShootingUnit->Attributes->GetAttackDamage()! %f"), ShootingUnit->Attributes->GetAttackDamage());
	if(UnitToHit && UnitToHit->TeamId != TeamId && ShootingUnit)
	{
		float NewDamage = ShootingUnit->Attributes->GetAttackDamage() - UnitToHit->Attributes->GetArmor();
			
		if(ShootingUnit->IsDoingMagicDamage)
			NewDamage = ShootingUnit->Attributes->GetAttackDamage() - UnitToHit->Attributes->GetMagicResistance();
			
		if(UnitToHit->Attributes->GetShield() <= 0)
			UnitToHit->SetHealth_Implementation(UnitToHit->Attributes->GetHealth()-NewDamage);
		else
			UnitToHit->SetShield_Implementation(UnitToHit->Attributes->GetShield()-NewDamage);
			//UnitToHit->Attributes->SetAttributeShield(UnitToHit->Attributes->GetShield()-NewDamage);


		if(UnitToHit && UnitToHit->GetUnitState() == UnitData::Dead)
		{
			// Do Nothing
		}else if(UnitToHit && UnitToHit->TeamId != TeamId)
		{
			UnitToHit->ApplyInvestmentEffect(ProjectileEffect);
		}
		
		ShootingUnit->IncreaseExperience();
		UnitToHit->ActivateAbilityByInputID(UnitToHit->DefensiveAbilityID, UnitToHit->DefensiveAbilities);

		SetNextBouncing(ShootingUnit, UnitToHit);
		SetBackBouncing(ShootingUnit);
	}			
}

void AProjectile::ImpactHeal_Implementation(AActor* ImpactTarget)
{
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Shooter);
	AUnitBase* UnitToHit = Cast<AUnitBase>(ImpactTarget);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile ShootingUnit->Attributes->GetAttackDamage()! %f"), ShootingUnit->Attributes->GetAttackDamage());
	if(UnitToHit && UnitToHit->TeamId == TeamId && ShootingUnit)
	{
		float NewDamage = ShootingUnit->Attributes->GetAttackDamage() ;
		
		if(UnitToHit->Attributes->GetShield() <= 0)
			UnitToHit->SetHealth_Implementation(UnitToHit->Attributes->GetHealth()+NewDamage);
		else
			UnitToHit->SetShield_Implementation(UnitToHit->Attributes->GetShield()+NewDamage);
			//UnitToHit->Attributes->SetAttributeShield(UnitToHit->Attributes->GetShield()+NewDamage);


		if(UnitToHit && UnitToHit->GetUnitState() == UnitData::Dead)
		{
			// Do Nothing
		}else if(UnitToHit && UnitToHit->TeamId == TeamId)
		{
			UnitToHit->ApplyInvestmentEffect(ProjectileEffect);
		}
		
		ShootingUnit->IncreaseExperience();
		UnitToHit->ActivateAbilityByInputID(UnitToHit->DefensiveAbilityID, UnitToHit->DefensiveAbilities);
		SetNextBouncing(ShootingUnit, UnitToHit);
		SetBackBouncing(ShootingUnit);
	}			
}


void AProjectile::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);
		if(UnitToHit && UnitToHit->GetUnitState() == UnitData::Dead)
		{
			ImpactEvent();
			DestroyProjectileWithDelay();
		}else if(UnitToHit && UnitToHit->TeamId == TeamId && BouncedBack && IsHealing)
		{
			ImpactEvent();
			ImpactHeal(Target);
			DestroyProjectileWithDelay();
		}else if(UnitToHit && UnitToHit->TeamId == TeamId && BouncedBack && !IsHealing)
		{
			ImpactEvent();
			DestroyProjectileWithDelay();
		}else if(UnitToHit && UnitToHit->TeamId != TeamId && !IsHealing)
		{
			ImpactEvent();
			Impact(Target);
			SetIsAttacked(UnitToHit);
			DestroyWhenMaxPierced();
		}else if(UnitToHit && UnitToHit->TeamId == TeamId && IsHealing)
		{
			ImpactEvent();
			ImpactHeal(Target);
			DestroyWhenMaxPierced();
		}
			
	}
}

void AProjectile::DestroyWhenMaxPierced()
{
	PiercedTargets += 1;
	if(PiercedTargets >= MaxPiercedTargets)
	{
		DestroyProjectileWithDelay();
	}
}

void AProjectile::DestroyProjectileWithDelay()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::DestroyProjectile, DestructionDelayTime, false);
}

void AProjectile::DestroyProjectile()
{
	Destroy(true, false);
}


void AProjectile::SetIsAttacked(AUnitBase* UnitToHit)
{
	if(UnitToHit->GetUnitState() != UnitData::Run &&
		UnitToHit->GetUnitState() != UnitData::Attack &&
		UnitToHit->GetUnitState() != UnitData::Casting &&
		UnitToHit->GetUnitState() != UnitData::Rooted &&
		UnitToHit->GetUnitState() != UnitData::Pause)
	{
		UnitToHit->UnitControlTimer = 0.f;
		UnitToHit->SetUnitState( UnitData::IsAttacked );
	}else if(UnitToHit->GetUnitState() == UnitData::Casting)
	{
		UnitToHit->UnitControlTimer -= UnitToHit->ReduceCastTime;
	}else if(UnitToHit->GetUnitState() == UnitData::Rooted)
	{
		UnitToHit->UnitControlTimer -= UnitToHit->ReduceRootedTime;
	}
}

void AProjectile::SetBackBouncing(AUnitBase* ShootingUnit)
{
	if(IsBouncingBack && IsBouncingNext && PiercedTargets == (MaxPiercedTargets-1))
	{
		Target = ShootingUnit;
		TargetLocation = ShootingUnit->GetActorLocation();
		BouncedBack = true;
	}else if(IsBouncingBack && PiercedTargets < MaxPiercedTargets)
	{
		Target = ShootingUnit;
		TargetLocation = ShootingUnit->GetActorLocation();
		BouncedBack = true;
	}
}

void AProjectile::SetNextBouncing(AUnitBase* ShootingUnit, AUnitBase* UnitToHit)
{
	if(IsBouncingNext)
	{
		AUnitBase* NewTarget = GetNextUnitInRange(ShootingUnit, UnitToHit);

		if(!NewTarget)
		{
			DestroyProjectileWithDelay();
			return;
		}
		
		Target = NewTarget;
		TargetLocation = NewTarget->GetActorLocation();
	}
}

AUnitBase* AProjectile::GetNextUnitInRange(AUnitBase* ShootingUnit, AUnitBase* UnitToHit)
{
	float Range = 9999999.f;
	AUnitBase* RUnit = nullptr; 
	for (AUnitBase* Unit : ShootingUnit->UnitsToChase)
	{
		if (Unit && Unit != UnitToHit)
		{
			float Distance = FVector::Dist(Unit->GetActorLocation(), ShootingUnit->GetActorLocation());
			if (Distance <= Range)
			{
				Range = Distance;
				RUnit = Unit;
			}else
			{
				
			}
		}
	}
	
	return RUnit;
}

void AProjectile::SetVisibility(bool Visible)
{
	GetMesh()->SetVisibility(Visible);
}

