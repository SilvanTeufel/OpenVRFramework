// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "HealingUnit.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AHealingUnit : public AUnitBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealActorSpawnOffset", Keywords = "OpenVRFramework HealActorSpawnOffset"), Category = OpenVRFramework)
	FVector HealActorSpawnOffset = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealActorScale", Keywords = "OpenVRFramework HealActorScale"), Category = OpenVRFramework)
	FVector HealActorScale = FVector(3.f,3.f,3.f);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnHealActor", Keywords = "OpenVRFramework SpawnHealActor"), Category = OpenVRFramework)
	void SpawnHealActor(AActor* Target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextUnitToChaseHeal", Keywords = "OpenVRFramework SetNextUnitToChaseHeal"), Category = OpenVRFramework)
	bool SetNextUnitToChaseHeal();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartHealingEvent();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastStartHealingEvent();
	
	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void StartHealingEvent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealingActorBaseClass", Keywords = "TopDownRTSTemplate HealingActorBaseClass"), Category = OpenVRFramework)
	TSubclassOf<class AHealingActor> HealingActorBaseClass;

	float HealTime = 0.5f;
	
};
