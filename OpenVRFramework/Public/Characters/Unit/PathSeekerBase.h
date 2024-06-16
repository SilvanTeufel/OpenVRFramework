// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GASUnit.h"
#include "LevelUnit.h"
#include "GameFramework/Character.h"
#include "Core/UnitData.h"
#include "Actors/DijkstraCenter.h"
#include "Core/DijkstraMatrix.h"
#include "PathSeekerBase.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API APathSeekerBase : public ALevelUnit
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector DijkstraStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector  DijkstraEndPoint;

	UPROPERTY()
	FDijkstraMatrix Next_DijkstraPMatrixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool  DijkstraSetPath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool  FollowPath = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool EnteredNoPathFindingArea = false;
	
};
