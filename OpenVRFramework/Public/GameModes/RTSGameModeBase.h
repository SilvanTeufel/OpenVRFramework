// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Waypoint.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInstance.h"
#include "Core/UnitData.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayEffect.h"
#include "Developer/GraphColor/Private/appconst.h"
#include "RTSGameModeBase.generated.h"


USTRUCT(BlueprintType)
struct FTimerHandleMapping
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Timer")
	int32 Id;

	UPROPERTY(VisibleAnywhere, Category = "Timer")
	FTimerHandle Timer;

	UPROPERTY(VisibleAnywhere, Category = "Timer")
	bool SkipTimer = false;
};
UCLASS()
class OPENVRFRAMEWORK_API ARTSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
//protected:
	public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	// Timer handle for spawning units
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	TArray<FTimerHandleMapping> SpawnTimerHandleMap;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	int TimerIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool DisableSpawn = true;
//public:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SetTeamId(int Id, ACameraControllerBase* CameraControllerBase);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SetTeamIds();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SetupTimerFromDataTable(FVector Location, AUnitBase* UnitToChase);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SetupUnitsFromDataTable(FVector Location, AUnitBase* UnitToChase, const TArray<class UDataTable*>& UnitTable); // , int TeamId, const FString& WaypointTag, int32 UnitIndex = 0, AUnitBase* SummoningUnit = nullptr, int SummonIndex = -1
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FTimerHandleMapping GetTimerHandleMappingById(int32 SearchId);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetSkipTimerMappingById(int32 SearchId, bool Value);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SpawnUnitFromDataTable(int id, FVector Location, AUnitBase* UnitToChase); // , int TeamId, AWaypoint* Waypoint = nullptr

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AUnitBase* SpawnSingleUnitFromDataTable(int id, FVector Location, AUnitBase* UnitToChase, int TeamId, AWaypoint* Waypoint = nullptr);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	bool IsUnitWithIndexDead(int32 UnitIndex);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	bool RemoveDeadUnitWithIndexFromDataSet(int32 UnitIndex);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	int32 CheckAndRemoveDeadUnits(int32 SpawnParaId);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AUnitBase* SpawnSingleUnits(FUnitSpawnParameter SpawnParameter, FVector Location, AUnitBase* UnitToChase, int TeamId, AWaypoint* Waypoint = nullptr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<int32> AvailableUnitIndexArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<int32> SpawnParameterIdArray;
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SpawnUnits(FUnitSpawnParameter SpawnParameter, FVector Location, AUnitBase* UnitToChase); // , int TeamId, AWaypoint* Waypoint = nullptr, int32 UnitIndex = 0, AUnitBase* SummoningUnit = nullptr, int SummonIndex = -1

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AddUnitIndexAndAssignToAllUnitsArray(AUnitBase* UnitBase);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AddUnitIndexAndAssignToAllUnitsArrayWithIndex(AUnitBase* UnitBase, int32 Index, FUnitSpawnParameter SpawnParameter);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AssignWaypointToUnit(AUnitBase* UnitBase, const FString& WaypointTag);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FVector CalcLocation(FVector Offset, FVector MinRange, FVector MaxRange);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<FUnitSpawnData> UnitSpawnDataSets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<class UDataTable*> UnitSpawnParameters;

};