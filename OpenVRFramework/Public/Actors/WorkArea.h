// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Characters/Unit/UnitBase.h"
#include "GameplayEffect.h"
#include "TimerManager.h"
#include "Core/WorkerData.h"
#include "Components/StaticMeshComponent.h"
#include "WorkArea.generated.h"


USTRUCT(BlueprintType)
struct FBuildingCost
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 PrimaryCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 SecondaryCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 TertiaryCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 RareCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 EpicCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int32 LegendaryCost = 0;
};


UCLASS()
class OPENVRFRAMEWORK_API AWorkArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorkArea();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FString Tag;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> AreaEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
	class UCapsuleComponent* TriggerCapsule;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void HandleResourceExtractionArea(AWorkingUnitBase* Worker, AUnitBase* UnitBase);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void HandleBaseArea(AWorkingUnitBase* Worker, AUnitBase* UnitBase, AResourceGameMode* ResourceGameMode, bool CanAffordConstruction);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SwitchResourceArea(AWorkingUnitBase* Worker, AUnitBase* UnitBase, AResourceGameMode* ResourceGameMode);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SwitchBuildArea(AWorkingUnitBase* Worker, AUnitBase* UnitBase, AResourceGameMode* ResourceGameMode);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void HandleBuildArea(AWorkingUnitBase* Worker, AUnitBase* UnitBase, AResourceGameMode* ResourceGameMode, bool CanAffordConstruction);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	EResourceType ConvertWorkAreaTypeToResourceType(WorkAreaData::WorkAreaType WorkAreaType);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DespawnWorkResource(AWorkResource* WorkResource);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TEnumAsByte<WorkAreaData::WorkAreaType> Type = WorkAreaData::Primary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<class AWorkResource> WorkResourceClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<class ABuildingBase> BuildingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<class AAIController> BuildingController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	ABuildingBase* Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BuildTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BuildZOffset = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool PlannedBuilding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool StartedBuilding = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FBuildingCost ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ResetStartBuildTime = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ControlTimer = 0.f;
	
	//UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	//bool CanAffordConstruction(int32 TeamId, int32 NumberOfTeams, TArray<FResourceArray> TeamResources);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class AWaypoint* NextWaypoint;
};
