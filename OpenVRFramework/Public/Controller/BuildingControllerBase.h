// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnitControllerBase.h"
#include "BuildingControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API ABuildingControllerBase : public AUnitControllerBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void BuildingControlStateMachine(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void CastingUnit(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AutoExecuteAbilitys(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void BuildingChase(AUnitBase* UnitBase, float DeltaSeconds);

	//UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	//void Patrol(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void PatrolRandomBuilding(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AttackBuilding(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void PauseBuilding(AUnitBase* UnitBase, float DeltaSeconds);
	//UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	//void PatrolIdle(AUnitBase* UnitBase, float DeltaSeconds);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<bool> AutoExeAbilitysArray = { true, false, false, false, false, false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float ExecutenDelayTime = 3.f;
	
};




