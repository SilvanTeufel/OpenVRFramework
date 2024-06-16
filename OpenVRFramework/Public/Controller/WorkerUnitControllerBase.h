// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnitControllerBase.h"
#include "WorkerUnitControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AWorkerUnitControllerBase : public AUnitControllerBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void WorkingUnitControlStateMachine(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void EvasionWorker(AUnitBase* UnitBase, FVector CollisionLocation);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GoToResourceExtraction(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void ResourceExtraction(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GoToBase(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GoToBuild(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FVector GetGroundLocation(FVector ALocation, AUnitBase* UnitBase);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void Build(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SpawnWorkResource(EResourceType ResourceType, FVector Location, TSubclassOf<class AWorkResource> WRClass, AUnitBase* ActorToLockOn);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DespawnWorkResource(AWorkResource* WorkResource);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DetachWorkResource(AWorkResource* WorkResource);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AUnitBase* SpawnSingleUnit(FUnitSpawnParameter SpawnParameter, FVector Location,
								AUnitBase* UnitToChase, int TeamId, AWaypoint* Waypoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ResetPathfindingTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BuildAreaArrivalDistance = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BaseArrivalDistance = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ResourceAreaArrivalDistance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool EnableDistanceCheck = false;
};





