// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "HUDBase.generated.h"



/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AHUDBase : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawHUD", Keywords = "OpenVRFramework DrawHUD"), Category = OpenVRFramework)
		virtual void DrawHUD(); // used in Tick();

	void Tick(float DeltaSeconds);

	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void AddUnitsToArray();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void AssignNewHighestIndex(AUnitBase* Unit);
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
		int32 HighestUnitIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "bSelectFriendly", Keywords = "TopDownRTSTemplate bSelectFriendly"), Category = OpenVRFramework)
		bool bSelectFriendly = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "InitialPoint", Keywords = "OpenVRFramework InitialPoint"), Category = OpenVRFramework)
		FVector2D InitialPoint; // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CurrentPoint", Keywords = "OpenVRFramework CurrentPoint"), Category = OpenVRFramework)
		FVector2D CurrentPoint; // Position of mouse on screen while holding;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IPoint", Keywords = "OpenVRFramework IPoint"), Category = OpenVRFramework)
		FVector IPoint = FVector(0.f,0.f, 0.f); // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CPoint", Keywords = "OpenVRFramework CPoint"), Category = OpenVRFramework)
		FVector CPoint = FVector(0.f,0.f, 0.f);
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RectangleScaleSelectionFactor", Keywords = "OpenVRFramework RectangleScaleSelectionFactor"), Category = OpenVRFramework)
		float RectangleScaleSelectionFactor = 0.9;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMousePos2D", Keywords = "OpenVRFramework GetMousePos2D"), Category = OpenVRFramework)
		FVector2D GetMousePos2D();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedEnemyUnits", Keywords = "OpenVRFramework SelectedEnemyUnits"), Category = OpenVRFramework)
	    TArray <AUnitBase*> SelectedUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "FriendlyUnits", Keywords = "OpenVRFramework FriendlyUnits"), Category = OpenVRFramework)
		TArray <AUnitBase*> FriendlyUnits;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "AllUnits", Keywords = "OpenVRFramework AllUnits"), Category = OpenVRFramework)
		TArray <AActor*> AllUnits;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
		TArray <AUnitBase*> EnemyUnitBases;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
		TArray <AUnitBase*> AllUnitBases;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
		TArray <AWorkingUnitBase*> WorkingUnits;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SpeakingUnits", Keywords = "OpenVRFramework SpeakingUnits"), Category = OpenVRFramework)
		TArray <ASpeakingUnit*> SpeakingUnits;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsSpeakingUnitClose", Keywords = "OpenVRFramework IsSpeakingUnitClose"), Category = OpenVRFramework)
		void IsSpeakingUnitClose(TArray <AUnitBase*> Units, TArray <ASpeakingUnit*> SpeakUnits);
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CharacterIsUnSelectable", Keywords = "OpenVRFramework CharacterIsUnSelectable"), Category = OpenVRFramework)
		bool CharacterIsUnSelectable = true;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveUnitsThroughWayPoints", Keywords = "OpenVRFramework MoveUnitsThroughWayPoints"), Category = OpenVRFramework)
		void MoveUnitsThroughWayPoints(TArray <AUnitBase*> Units);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PatrolUnitsThroughWayPoints", Keywords = "OpenVRFramework PatrolUnitsThroughWayPoints"), Category = OpenVRFramework)
		void PatrolUnitsThroughWayPoints(TArray <AUnitBase*> Units);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitSelected", Keywords = "OpenVRFramework SetUnitSelected"), Category = OpenVRFramework)
		void SetUnitSelected(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitSelected", Keywords = "OpenVRFramework SetUnitSelected"), Category = OpenVRFramework)
		void DeselectAllUnits();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void DetectUnit(AUnitBase* DetectingUnit, TArray<AActor*>& DetectedUnits, float Sight);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ControllDirectionToMouse", Keywords = "OpenVRFramework ControllDirectionToMouse"), Category = OpenVRFramework)
		void ControllDirectionToMouse(AActor* Units, FHitResult Hit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsActorInsideRectangle", Keywords = "OpenVRFramework IsActorInsideRectangle"), Category = OpenVRFramework)
		bool IsActorInsideRec(FVector InPoint, FVector CuPoint, FVector ALocation);

};
