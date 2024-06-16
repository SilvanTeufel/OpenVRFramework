// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RTSGameModeBase.h"
#include "Core/WorkerData.h"
#include "Actors/WorkArea.h"
#include "Characters/Unit/WorkingUnitBase.h"
#include "ResourceGameMode.generated.h"

/**
 * GameMode class that manages resources for teams in a more generic way.
 */

USTRUCT(BlueprintType)
struct FWorkAreaArrays
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> PrimaryAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> SecondaryAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> TertiaryAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> RareAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> EpicAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> LegendaryAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> BaseAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<AWorkArea*> BuildAreas;
};

UCLASS()
class OPENVRFRAMEWORK_API AResourceGameMode : public ARTSGameModeBase
{
	GENERATED_BODY()

public:
	// Constructor
	AResourceGameMode();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override; // Override BeginPlay
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Work)
	FWorkAreaArrays WorkAreaGroups; // Storage for work areas grouped by type

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Work)
	int MaxResourceAreasToSet = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Work)
	int MaxBuildAreasToSet = 15;
	
	// Helper function to initialize resource arrays
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void InitializeResources(int32 NumberOfTeams);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GatherWorkAreas();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AssignWorkAreasToWorkers();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AWorkArea* GetClosestWorkArea(AWorkingUnitBase* Worker, const TArray<AWorkArea*>& WorkAreas);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	TArray<AWorkArea*> GetClosestBase(AWorkingUnitBase* Worker);
	
public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Work)
	int32 NumberOfTeams = 10;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	TArray<AWorkArea*> GetFiveClosestResourcePlaces(AWorkingUnitBase* Worker);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	TArray<AWorkArea*> GetClosestBuildPlaces(AWorkingUnitBase* Worker);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AWorkArea* GetRandomClosestWorkArea(const TArray<AWorkArea*>& WorkAreas);
	// Function to modify a resource for a specific team
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void ModifyResource(EResourceType ResourceType, int32 TeamId, float Amount);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	float GetResource(int TeamId, EResourceType RType);
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Work)
	TArray<FResourceArray> TeamResources;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	bool CanAffordConstruction(const FBuildingCost& ConstructionCost, int32 TeamId) const;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AssignWorkAreasToWorker(AWorkingUnitBase* Worker);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AWorkArea* GetSuitableWorkAreaToWorker(int TeamId, const TArray<AWorkArea*>& WorkAreas);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	TArray<AWorkArea*> GetClosestResourcePlaces(AWorkingUnitBase* Worker);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AddCurrentWorkersForResourceType(int TeamId, EResourceType ResourceType, float Amount);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	int32 GetCurrentWorkersForResourceType(int TeamId, EResourceType ResourceType) const;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AddMaxWorkersForResourceType(int TeamId, EResourceType ResourceType, float Amount);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	int32 GetMaxWorkersForResourceType(int TeamId, EResourceType ResourceType) const;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetCurrentWorkersForResourceType(int TeamId, EResourceType ResourceType, float Amount);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetAllCurrentWorkers(int TeamId);
	
};