// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Waypoint.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStartBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API APlayerStartBase : public APlayerStart
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= OpenVRFramework)
	int SelectableTeamId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	AWaypoint* DefaultWaypoint;
};
