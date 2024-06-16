// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

#include "Characters/Unit/UnitBase.h"
#include "Hud/HUDBase.h"
#include "Hud/PathProviderHUD.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "NavMesh/NavMeshPath.h"


#include "UnitControllerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveCompletedDelegate, FAIRequestID, RequestID, EPathFollowingResult::Type, Result);
/**
 * 
 */

UCLASS()
class OPENVRFRAMEWORK_API AUnitControllerBase : public AAIController
{
	GENERATED_BODY()

private:
	AUnitBase* PendingUnit = nullptr;
	FVector PendingDestination;
	
public:
	AUnitControllerBase();
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual FRotator GetControlRotation() const override;

	FOnMoveCompletedDelegate OnMoveCompleted;
	
	void OnAdjustedMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
	{
		if(PendingUnit &&  Result == EPathFollowingResult::Success) //  Result.IsSuccess()
		{
			MoveToLocationUEPathFinding(PendingUnit, PendingDestination);
			// Reset the PendingUnit and PendingDestination to avoid reusing them incorrectly
			PendingUnit = nullptr;
		}
	};

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "KillUnitBase", Keywords = "OpenVRFramework KillUnitBase"), Category = OpenVRFramework)
		void KillUnitBase(AUnitBase* UnitBase);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SightRadius", Keywords = "OpenVRFramework SightRadius"), Category = OpenVRFramework)
		float SightRadius = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SightAge", Keywords = "OpenVRFramework SightAge"), Category = OpenVRFramework)
		float SightAge = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoseSightRadius", Keywords = "OpenVRFramework LoseSightRadius"), Category = OpenVRFramework)
		float LoseSightRadius = SightRadius + 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FieldOfView", Keywords = "OpenVRFramework FieldOfView"), Category = OpenVRFramework)
		float FieldOfView = 360.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DespawnTime", Keywords = "OpenVRFramework DespawnTime"), Category = OpenVRFramework)
		float DespawnTime = 4.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PauseDuration", Keywords = "OpenVRFramework PauseDuration"), Category = OpenVRFramework)
		//float PauseDuration = 0.6f; // Duratin of the State Pause
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDuration", Keywords = "OpenVRFramework AttackDuration"), Category = OpenVRFramework)
		float AttackDuration = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsAttackedDuration", Keywords = "OpenVRFramework IsAttackedDuration"), Category = OpenVRFramework)
		float IsAttackedDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float IsRootedDuration = 5.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		//float IsCastingDuration = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackAngleTolerance", Keywords = "OpenVRFramework AttackAngleTolerance"), Category = OpenVRFramework)
		float AttackAngleTolerance = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SightConfig", Keywords = "OpenVRFramework SightConfig"), Category = OpenVRFramework)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DistanceToUnitToChase", Keywords = "OpenVRFramework DistanceToUnitToChase"), Category = OpenVRFramework)
		float DistanceToUnitToChase = 0.0f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DetectFriendlyUnits", Keywords = "OpenVRFramework DetectFriendlyUnits"), Category = OpenVRFramework)
		bool DetectFriendlyUnits = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnUnitDetected", Keywords = "OpenVRFramework OnUnitDetected"), Category = OpenVRFramework)
		void OnUnitDetected(const TArray<AActor*>& DetectedUnits);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateToAttackUnit", Keywords = "OpenVRFramework RotateToAttackUnit"), Category = OpenVRFramework)
		void RotateToAttackUnit(AUnitBase* AttackingUnit, AUnitBase* UnitToAttack);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnitControlStateMachine", Keywords = "OpenVRFramework UnitControlStateMachine"), Category = OpenVRFramework)
		void UnitControlStateMachine(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void Rooted(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void Casting(AUnitBase* UnitBase, float DeltaSeconds);

	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Dead", Keywords = "OpenVRFramework Dead"), Category = OpenVRFramework)
		bool IsUnitToChaseInRange(AUnitBase* UnitBase);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Dead", Keywords = "OpenVRFramework Dead"), Category = OpenVRFramework)
		void Dead(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Patrol", Keywords = "OpenVRFramework Patrol"), Category = OpenVRFramework)
		void Patrol(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Run", Keywords = "OpenVRFramework Run"), Category = OpenVRFramework)
		void Run(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Chase", Keywords = "OpenVRFramework Chase"), Category = OpenVRFramework)
		void Chase(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void ActivateCombatAbilities(AUnitBase* UnitBase);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		FVector CalculateChaseLocation(AUnitBase* UnitBase);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void LoseUnitToChase(AUnitBase* UnitBase);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void ResetPath(AUnitBase* UnitBase);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Attack", Keywords = "OpenVRFramework Attack"), Category = OpenVRFramework)
		void Attack(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pause", Keywords = "OpenVRFramework Pause"), Category = OpenVRFramework)
		void Pause(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsAttacked", Keywords = "OpenVRFramework IsAttacked"), Category = OpenVRFramework)
		void IsAttacked(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Idle", Keywords = "OpenVRFramework Idle"), Category = OpenVRFramework)
		void Idle(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void EvasionChase(AUnitBase* UnitBase, FVector CollisionLocation);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void EvasionIdle(AUnitBase* UnitBase, FVector CollisionLocation);

	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetUnitBackToPatrol(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void RunUEPathfinding(AUnitBase* UnitBase, float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool SetUnitsBackToPatrol = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SetUnitsBackToPatrolTime = 10.f;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void PatrolUEPathfinding(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FVector GetCloseLocation(FVector ToLocation, float Distance);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetPatrolCloseLocation(AUnitBase* UnitBase);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetUEPathfindingRandomLocation(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		bool SetUEPathfinding(AUnitBase* UnitBase, float DeltaSeconds, FVector Location);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetUEPathfindingTo(AUnitBase* UnitBase, float DeltaSeconds, FVector Location);

	//UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	//bool MoveToLocationUEPathFinding(AUnitBase* Unit, const FVector& DestinationLocation);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	bool MoveToLocationUEPathFinding(AUnitBase* Unit, const FVector& DestinationLocation);
	
		//void OnAdjustedMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "CreateProjectile", Keywords = "OpenVRFramework CreateProjectile"), Category = OpenVRFramework)
		void CreateProjectile (AUnitBase* UnitBase);

	UPROPERTY(BlueprintReadWrite,  Category = OpenVRFramework)
		bool ProjectileSpawned = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		float AcceptanceRadius = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		bool DebugPatrolRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		UClass* SurfaceClass = AStaticMeshActor::StaticClass();
};

