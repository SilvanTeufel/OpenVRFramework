// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "PathSeekerBase.h"
#include "GameFramework/Character.h"
#include "Actors/Pickup.h"
#include "Engine/World.h"
#include "SpawnerUnit.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<APickup> PickupBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ProbabilityArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxLifeTime;
};

UCLASS()
class OPENVRFRAMEWORK_API ASpawnerUnit : public ACharacter
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	UDataTable* SpawnDataTable;
	
	UPROPERTY(BlueprintReadWrite, Category = Spawn)
	TArray<FSpawnData> SpawnDataArray;

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void CreateSpawnDataFromDataTable();
	
	UFUNCTION(BlueprintCallable, Category = Spawn)
	APickup* SpawnPickup(FVector Location, FSpawnData Data);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	bool SpawnPickupWithProbability(FSpawnData Data, FVector Offset);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void SpawnPickupsArray();

	UPROPERTY(BlueprintReadWrite, Category = Spawn)
	bool IsSpawned = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int TeamId = 1;
};
