// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkingUnitBase.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Core/UnitData.h"
#include "PathSeekerBase.h"
#include "UnitBase.generated.h"


UCLASS()
class OPENVRFRAMEWORK_API AUnitBase : public AWorkingUnitBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FString Name = "Unit";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	bool IsPlayer = false;
	
	AUnitBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	AUnitBase* CollisionUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer UnitTags;
	
protected:
// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FRotator ServerMeshRotation = FRotator(0.f, -90.f, 0.f);
	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing=OnRep_MeshAssetPath, Category = OpenVRFramework)
	FString MeshAssetPath;

	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing=OnRep_MeshMaterialPath, Category = OpenVRFramework)
	FString MeshMaterialPath;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnRep_MeshAssetPath();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnRep_MeshMaterialPath();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetMeshRotationServer();
	
//	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TeamId", Keywords = "OpenVRFramework IsFriendly"), Category = OpenVRFramework)
		//int TeamId = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsFlying", Keywords = "OpenVRFramework IsFlying"), Category = OpenVRFramework)
		bool IsFlying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FlyHeight", Keywords = "OpenVRFramework FlyHeight"), Category = OpenVRFramework)
		float FlyHeight = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool UEPathfindingUsed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool UsingUEPathfindingPatrol = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool SetUEPathfinding = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		//bool ReCalcRandomLocation = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float AutoSetUEPathfindingTimer = 0.f;
	
// RTSHud related //////////////////////////////////////////
public:

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackEvent();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastStartAttackEvent();
	
	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void StartAttackEvent();


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMeeleImpactEvent();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastMeeleImpactEvent();
	
	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void MeeleImpactEvent();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "OpenVRFramework CreateCameraComp"), Category = OpenVRFramework)
	void IsAttacked(AActor* AttackingCharacter); // AActor* SelectedCharacter

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SetRunLocation(FVector Location);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "OpenVRFramework CreateCameraComp"), Category = OpenVRFramework)
	void SetWalkSpeed(float Speed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopRunTolerance", Keywords = "OpenVRFramework StopRunTolerance"), Category = OpenVRFramework)
		float StopRunTolerance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopRunToleranceY", Keywords = "OpenVRFramework StopRunToleranceY"), Category = OpenVRFramework)
		float StopRunToleranceForFlying = 100.f;
///////////////////////////////////////////////////////////////////

// related to Animations  //////////////////////////////////////////
public:

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitControlTimer", Keywords = "OpenVRFramework UnitControlTimer"), Category = OpenVRFramework)
	float UnitControlTimer = 0.0f; // This Timer is used in UnitControllerBase Statemachine

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	bool ToggleUnitDetection = false;

	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "OpenVRFramework CreateCameraComp"), Category = OpenVRFramework)
	void SetToggleUnitDetection(bool ToggleTo);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	bool GetToggleUnitDetection();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitToChase", Keywords = "OpenVRFramework UnitToChase"), Category = OpenVRFramework)
	AUnitBase* UnitToChase;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitsToChase", Keywords = "OpenVRFramework UnitsToChase"), Category = OpenVRFramework)
	TArray <AUnitBase*> UnitsToChase;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextUnitToChase", Keywords = "OpenVRFramework SetNextUnitToChase"), Category = OpenVRFramework)
	bool SetNextUnitToChase();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DistanceToUnitToChase", Keywords = "OpenVRFramework DistanceToUnitToChase"), Category = OpenVRFramework)
	float DistanceToUnitToChase;
///////////////////////////////////////////////////////

// related to Waypoints  //////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "NextWaypoint", Keywords = "OpenVRFramework NextWaypoint"), Category = OpenVRFramework)
	class AWaypoint* NextWaypoint;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetWaypoint", Keywords = "OpenVRFramework SetWaypoint"), Category = OpenVRFramework)
	void SetWaypoint(class AWaypoint* NewNextWaypoint);
///////////////////////////////////////////////////////////////////



// related to Healthbar  //////////////////////////////////////////
public:

	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "SetHealth", Keywords = "OpenVRFramework SetHealth"), Category = OpenVRFramework)
	void SetHealth(float NewHealth);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealthWidgetComp", Keywords = "OpenVRFramework HealthWidgetComp"), Category = OpenVRFramework)
		class UWidgetComponent* HealthWidgetComp;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealthWidgetCompLocation", Keywords = "OpenVRFramework HealthWidgetCompLocation"), Category = OpenVRFramework)
		FVector HealthWidgetCompLocation = FVector (0.f, 0.f, 180.f);
///////////////////////////////////////////////////////////////////


// HUDBase related ///////////
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSelected", Keywords = "TopDownRTSTemplate SetSelected"), Category = OpenVRFramework)
	void SetSelected();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDeselected", Keywords = "TopDownRTSTemplate SetDeselected"), Category = OpenVRFramework)
	void SetDeselected();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RunLocationArray", Keywords = "OpenVRFramework RunLocationArray"), Category = OpenVRFramework)
	TArray <FVector> RunLocationArray;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RunLocationArrayIterator", Keywords = "OpenVRFramework RunLocationArrayIterator"), Category = OpenVRFramework)
	int32 RunLocationArrayIterator = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RunLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RandomPatrolLocation;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float LineTraceZDistance = 80.f;
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float EvadeDistance = 70.f;
/////////////////////////////

// SelectedIcon related /////////
protected:
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedCharacterIcon", Keywords = "OpenVRFramework SelectedCharacterIcon"), Category = OpenVRFramework)
	class ASelectedIcon* SelectedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<class ASelectedIcon> SelectedIconBaseClass;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnSelectedIcon", Keywords = "OpenVRFramework SpawnSelectedIcon"), Category = OpenVRFramework)
	void SpawnSelectedIcon();
//////////////////////////////////////


// Projectile related /////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileBaseClass", Keywords = "TopDownRTSTemplate ProjectileBaseClass"), Category = OpenVRFramework)
	TSubclassOf<class AProjectile> ProjectileBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Projectile", Keywords = "OpenVRFramework Projectile"), Category = OpenVRFramework)
	class AProjectile* Projectile;

	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "SpawnProjectile", Keywords = "OpenVRFramework SpawnProjectile"), Category = OpenVRFramework)
	void SpawnProjectile(AActor* Target, AActor* Attacker);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SpawnProjectileFromClass(AActor* Aim, AActor* Attacker, TSubclassOf<class AProjectile> ProjectileClass, int MaxPiercedTargets, bool FollowTarget, int ProjectileCount, float Spread, bool IsBouncingNext, bool IsBouncingBack, bool DisableAutoZOffset, float ZOffset, float Scale = 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UseProjectile", Keywords = "OpenVRFramework UseProjectile"), Category = OpenVRFramework)
	bool UseProjectile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileSpawnOffset", Keywords = "OpenVRFramework ProjectileSpawnOffset"), Category = OpenVRFramework)
	FVector ProjectileSpawnOffset = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileScale", Keywords = "OpenVRFramework ProjectileScale"), Category = OpenVRFramework)
	FVector ProjectileScale = FVector(1.f,1.f,1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FRotator ProjectileRotationOffset = FRotator(0.f,90.f,-90.f);
	//////////////////////////////////////
	
	
// Used for Despawn  //////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DestroyAfterDeath", Keywords = "OpenVRFramework DestroyAfterDeath"), Category = OpenVRFramework)
	bool DestroyAfterDeath = true;
///////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PauseDuration", Keywords = "OpenVRFramework PauseDuration"), Category = OpenVRFramework)
		float PauseDuration = 0.6f;
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = OpenVRFramework)
		float CastTime = 5.f;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = OpenVRFramework)
		float ReduceCastTime = 0.5f;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = OpenVRFramework)
		float ReduceRootedTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		class UWidgetComponent* TimerWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		FVector TimerWidgetCompLocation = FVector (0.f, 0.f, -180.f);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetupTimerWidget();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetTimerWidgetCastingColor(FLinearColor Color);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TArray<FUnitSpawnData> SummonedUnitsDataSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TArray<int> SummonedUnitIndexes = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	UFUNCTION(BlueprintCallable, Category = Ability)
	int SpawnUnitsFromParameters(
TSubclassOf<class AAIController> AIControllerBaseClass,
TSubclassOf<class AUnitBase> UnitBaseClass, UMaterialInstance* Material, USkeletalMesh* CharacterMesh, FRotator HostMeshRotation, FVector Location,
TEnumAsByte<UnitData::EState> UState,
TEnumAsByte<UnitData::EState> UStatePlaceholder,
int NewTeamId, AWaypoint* Waypoint, int UIndex);

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool IsSpawnedUnitDead(int UIndex);
	
	UFUNCTION(BlueprintCallable, Category = Ability)
	void SetUnitBase(int UIndex, AUnitBase* NewUnit);
};








