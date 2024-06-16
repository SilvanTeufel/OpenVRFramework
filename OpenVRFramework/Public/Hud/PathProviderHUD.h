// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hud/HUDBase.h"
#include "Core/UnitData.h"
#include "Core/DijkstraMatrix.h"
#include "Math/Matrix.h"
#include "Engine/DataTable.h"
#include "Actors/DijkstraCenter.h"
#include "Actors/NoPathFindingArea.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "DrawDebugHelpers.h"
#include "PathProviderHUD.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API APathProviderHUD : public AHUDBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	FCollisionQueryParams QueryParams;
	
	void BeginPlay() override;
	
	void Tick(float DeltaSeconds);

	
	UPROPERTY(EditAnywhere, Category= OpenVRFramework)
	uint64 MaxCosts = 99999.f;

	UPROPERTY(EditAnywhere, Category= OpenVRFramework)
	float MaxDistance = 99999.f;

	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	bool Debug = true;

	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	bool StopLoading = true;
	
	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	bool DisablePathFindingOnEnemy = true;

	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	bool DisablePathFindingOnFriendly = true;
	
	UPROPERTY(EditAnywhere, Category = OpenVRFramework)
	TArray<FPathMatrix> PathMatrixes;
	
	// Create the Grid for Dijkstra ///////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FoundCenterPoints", Keywords = "OpenVRFramework FoundCenterPoints"), Category = OpenVRFramework)
	TArray<AActor*> FoundCenterPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class UDataTable* GridDataTable;

	UPROPERTY(BlueprintReadWrite, Category= OpenVRFramework)
	bool CreatedGridAndDijkstra = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateGridAndDijkstra", Keywords = "OpenVRFramework CreateGridAndDijkstra"), Category = OpenVRFramework)
	void CreateGridAndDijkstra();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateGridsfromDataTable", Keywords = "OpenVRFramework CreateGridsfromDataTable"), Category = OpenVRFramework)
	void CreateGridsfromDataTable();
	
	UFUNCTION(meta = (DisplayName = "CreatePathMatrix", Keywords = "OpenVRFramework CreatePathMatrix"), Category = OpenVRFramework)
	TArray<FPathMatrixRow> CreatePathMatrix(int ColCount, int RowCount, float Delta, FVector Offset);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////


	// Calc Dijkstra in Begin_Play ///////////////////////////////////////////////////////////////////////
	
	TArray<FDijkstraMatrix> G_DijkstraPMatrixes;

	UPROPERTY(EditAnywhere, Category= OpenVRFramework)
	TArray<FPathPoint> Done;

	UFUNCTION(meta = (DisplayName = "Dijkstra", Keywords = "OpenVRFramework Dijkstra"), Category = OpenVRFramework)
	TArray<FDijkstraRow> Dijkstra(FVector CenterPoint, TArray<FPathMatrixRow> PMatrix);
	
	UFUNCTION(meta = (DisplayName = "DijkstraInit", Keywords = "OpenVRFramework DijkstraInit"), Category = OpenVRFramework)
	TArray<FDijkstraRow> DijkstraInit(TArray<FPathMatrixRow> PMatrix, TArray<FPathPoint>& Ques, int StartId, FVector3d StartPoint);

	UFUNCTION(meta = (DisplayName = "CheckDistrajkLoop", Keywords = "OpenVRFramework CheckDistrajkLoop"), Category = OpenVRFramework)
	bool CheckDijkstraLoop(TArray<FDijkstraRow>& DMatrix, int k, TArray<FPathMatrixRow> PMatrix, int u, TArray<FPathPoint>& NewQue,TArray<FPathPoint> DoneQue, FPathPoint CurrentQue);

	////////////////////////////////////////////////////////////////////////////////////////////////////


	// Handle Dijkstra Matrix ///////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, Category= OpenVRFramework)
	int MaxPathIteration = 5000;
	
	UFUNCTION(meta = (DisplayName = "GetStartPoint", Keywords = "OpenVRFramework GetStartPoint"), Category = OpenVRFramework)
	void GetStartPoint(TArray<FPathMatrixRow> PMatrix ,int& StartId , float& ShortestDistanceToStart, FVector3d StartPoint);

	UFUNCTION(meta = (DisplayName = "GetEndPoint", Keywords = "OpenVRFramework GetEndPoint"), Category = OpenVRFramework)
	void GetEndPoint(TArray<FDijkstraRow> DMatrix, float& ShortestDistanceToEnd, int& EndId, FVector3d EndPoint);

	UFUNCTION(meta = (DisplayName = "CreatePathMatrix", Keywords = "OpenVRFramework CreatePathMatrix"), Category = OpenVRFramework)
	bool IsPointInsideArray(TArray<FPathPoint> Array, FPathPoint Point);

	UFUNCTION(meta = (DisplayName = "IsIdInsideMatrix", Keywords = "OpenVRFramework IsIdInsideMatrix"), Category = OpenVRFramework)
	bool IsIdInsideMatrix(TArray<FDijkstraRow> Matrix, int Id);

	UFUNCTION(meta = (DisplayName = "GivePointFromID", Keywords = "OpenVRFramework GivePointFromID"), Category = OpenVRFramework)
	FDijkstraRow GivePointFromID(TArray<FDijkstraRow> Matrix, int Id);

	////////////////////////////////////////////////////////////////////////////////////////////////////


	// On Real-Time ///////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SetNextDijkstraPauseTime", Keywords = "OpenVRFramework SetNextDijkstraPauseTime"), Category = OpenVRFramework)
	float SetNextDijkstraPauseTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlTimer", Keywords = "OpenVRFramework ControlTimer"), Category = OpenVRFramework)
	float ControlTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StartTimer", Keywords = "OpenVRFramework StartTimer"), Category = OpenVRFramework)
	float StartTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StartTime", Keywords = "OpenVRFramework StartTime"), Category = OpenVRFramework)
	float StartTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UseDijkstraOnlyOnFirstUnit", Keywords = "OpenVRFramework UseDijkstraOnlyOnFirstUnit"), Category = OpenVRFramework)
	bool UseDijkstraOnlyOnFirstUnit = true;
	
	UFUNCTION(meta = (DisplayName = "GetDistrajkPath", Keywords = "OpenVRFramework GetDistrajkPath"), Category = OpenVRFramework)
	TArray<FPathPoint> GetDijkstraPath(TArray<FDijkstraRow>& DMatrix, int EndId);
	
	UFUNCTION(meta = (DisplayName = "GetPathReUseDijkstra", Keywords = "OpenVRFramework GetPathReUseDijkstra"), Category = OpenVRFramework)
	TArray<FPathPoint> GetPathReUseDijkstra(TArray<FDijkstraRow> DMatrix, FVector3d EndPoint, FVector3d StartPoint);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// SET Dijkstra on Character ///////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextDijkstra", Keywords = "OpenVRFramework SetNextDijkstra"), Category = OpenVRFramework)
	void SetNextDijkstra(TArray <AUnitBase*> Units, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextDijkstraTo", Keywords = "OpenVRFramework SetNextDijkstraTo"), Category = OpenVRFramework)
	void SetNextDijkstraTo(TArray <AUnitBase*> Units, FVector Location );

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDijkstraWithClosestZDistance", Keywords = "OpenVRFramework SetDijkstraWithClosestZDistance"), Category = OpenVRFramework)
	void SetDijkstraWithClosestZDistance(AUnitBase* UnitBase, float Z);////

	////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FoundCenterPoints", Keywords = "OpenVRFramework FoundCenterPoints"), Category = OpenVRFramework)
	TArray<ANoPathFindingArea*> NoPathFindingAreas;
	
	UPROPERTY(EditAnywhere, Category= OpenVRFramework)
	float RangeThreshold = 500.f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Location", Keywords = "OpenVRFramework Location"), Category = OpenVRFramework)
	bool IsLocationInNoPathFindingAreas(FVector Location);
};
