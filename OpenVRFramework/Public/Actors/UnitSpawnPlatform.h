// Copyright 2024 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Core/UnitData.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "UnitSpawnPlatform.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AUnitSpawnPlatform : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AUnitSpawnPlatform(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FTimerHandle RespawnTimerHandle;
	FTimerHandle PositionTimerHandle;
	FTimerHandle EnergyTimerHandle;
public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void UpdateUnitPositions();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void UpdateEnergy();
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void SpawnUnitsFromArray();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void ReSpawnUnits();
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	int SpawnUnit(
	TSubclassOf<class AAIController> AIControllerBaseClass,
	TSubclassOf<class AUnitBase> UnitBaseClass, UMaterialInstance* Material, USkeletalMesh* CharacterMesh, FRotator HostMeshRotation, FVector Location,
	TEnumAsByte<UnitData::EState> UState,
	TEnumAsByte<UnitData::EState> UStatePlaceholder,
	int NewTeamId, AWaypoint* Waypoint, int UIndex);

private:
	// Pointer to the Static Mesh Component
	UPROPERTY(VisibleAnywhere, Category=RTSUnitTemplate)
	UStaticMeshComponent* PlatformMesh;

	// Scene component to specify spawn location
	UPROPERTY(VisibleAnywhere, Category=RTSUnitTemplate)
	USceneComponent* SpawnPoint;

	
public:

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void HideOnTeamId(int PCTeamId);
	// Variables for specifying spawn details
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TArray<TSubclassOf<AUnitBase>> DefaultUnitBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TArray<TSubclassOf<AAIController>> DefaultAIControllerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	UMaterialInstance* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	USkeletalMesh* DefaultCharacterMesh;

	// Default values for other spawn parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	FRotator DefaultHostMeshRotation = FRotator(0.0f, -90.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	FVector DefaultLocationOffset = FVector(0.0f, 0.0f, 50.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TEnumAsByte<UnitData::EState> DefaultState = UnitData::EState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TEnumAsByte<UnitData::EState> DefaultStatePlaceholder = UnitData::EState::Idle;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	int TeamId = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	AWaypoint* DefaultWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	int DefaultUIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TArray<AUnitBase*> SpawnedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float UnitSpacing = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float UnitZOffset = 50.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	class UWidgetComponent* EnergyWidgetComp;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float Energy = 100.f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float MaxEnergy = 100.f;

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	float GetEnergy();
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	float GetMaxEnergy();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void SetEnergy(float NewEnergy);

};