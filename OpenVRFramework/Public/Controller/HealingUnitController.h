// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller/SpeakingUnitControllerBase.h"
#include "Characters/Unit/HealingUnit.h"
#include "HealingUnitController.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AHealingUnitController : public ASpeakingUnitControllerBase
{
	GENERATED_BODY()
public:
	AHealingUnitController();
	/*
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;
	*/
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnitControlStateMachine", Keywords = "OpenVRFramework UnitControlStateMachine"), Category = OpenVRFramework)
		void HealingUnitControlStateMachine(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChaseHealTarget", Keywords = "OpenVRFramework ChaseHealTarget"), Category = OpenVRFramework)
		void ChaseHealTarget(AHealingUnit* UnitBase,float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Healing", Keywords = "OpenVRFramework Healing"), Category = OpenVRFramework)
		void Healing(AHealingUnit* UnitBase,float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HealPause", Keywords = "OpenVRFramework HealPause"), Category = OpenVRFramework)
		void HealPause(AHealingUnit* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HealRun", Keywords = "OpenVRFramework HealRun"), Category = OpenVRFramework)
		void HealRun(AHealingUnit* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void HealRunUEPathfinding(AHealingUnit* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HealPatrol", Keywords = "OpenVRFramework HealPatrol"), Category = OpenVRFramework)
		void HealPatrol(AHealingUnit* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void HealPatrolUEPathfinding(AHealingUnit* UnitBase, float DeltaSeconds);
	
	bool bHealActorSpawned = false;
};
