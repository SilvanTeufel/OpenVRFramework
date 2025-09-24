// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GASUnit.h"
#include "PerformanceUnit.h"
#include "GameFramework/Character.h"
#include "Core/UnitData.h"
#include "PathSeekerBase.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API APathSeekerBase : public APerformanceUnit
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
