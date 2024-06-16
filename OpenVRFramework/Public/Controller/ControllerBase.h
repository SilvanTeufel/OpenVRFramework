// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hud/PathProviderHUD.h"
#include "Characters/Camera/CameraBase.h"
#include "Core/UnitData.h"
#include "GameFramework/PlayerController.h"
#include "Actors/EffectArea.h"
#include "EOS/EOS_PlayerController.h"
#include "ControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AControllerBase : public AEOS_PlayerController
{
	GENERATED_BODY()

public:
	AControllerBase();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	

	// Timer handle for managing FPS display updates
	FTimerHandle FPSTimerHandle;

	// Function called by timer to display FPS
	void DisplayFPS();

	UFUNCTION(BlueprintCallable, Category="Display")
	void ToggleFPSDisplay(bool bEnable);
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	APathProviderHUD* HUDBase;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	ACameraBase* CameraBase;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	AActor* ClickedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool ShowFPS = true;
	//UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	//FVector ClickLocation = FVector(0.0f, 0.0f, 0.0f);
	
	void Tick(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShiftPressed", Keywords = "OpenVRFramework ShiftPressed"), Category = OpenVRFramework)
		void ShiftPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShiftReleased", Keywords = "OpenVRFramework ShiftReleased"), Category = OpenVRFramework)
		void ShiftReleased();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SelectUnit(int Index);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LeftClickAMoveUEPF(AUnitBase* Unit, FVector Location);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LeftClickAMove(AUnitBase* Unit, FVector Location);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LeftClickAttack(AUnitBase* Unit, FVector Location);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LeftClickSelect();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LeftClickPressed", Keywords = "OpenVRFramework LeftClickPressed"), Category = OpenVRFramework)
		void LeftClickPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LeftClickReleased", Keywords = "OpenVRFramework LeftClickReleased"), Category = OpenVRFramework)
		void LeftClickReleased();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetWidgets(int Index);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SetRunLocation(AUnitBase* Unit, const FVector& DestinationLocation);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void MoveToLocationUEPathFinding(AUnitBase* Unit, const FVector& DestinationLocation);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SetUnitState_Replication(AUnitBase* Unit, int State);
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "IsShiftPressed", Keywords = "OpenVRFramework IsShiftPressed"), Category = OpenVRFramework)
		bool UseUnrealEnginePathFinding = true;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
		float UEPathfindingCornerOffset = 100.f;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SetToggleUnitDetection(AUnitBase* Unit, bool State);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void RightClickRunShift(AUnitBase* Unit, FVector Location);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void RightClickRunUEPF(AUnitBase* Unit, FVector Location);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void RightClickRunDijkstraPF(AUnitBase* Unit, FVector Location, int Counter);

	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RightClickPressed", Keywords = "OpenVRFramework RightClickPressed"), Category = OpenVRFramework)
		void RightClickPressed();

	UFUNCTION(meta = (DisplayName = "SetRunLocationUseDijkstra", Keywords = "OpenVRFramework SetRunLocationUseDijkstra"), Category = OpenVRFramework)
		void SetRunLocationUseDijkstra(FVector HitLocation, FVector UnitLocation, TArray <AUnitBase*> Units, TArray<FPathPoint>& PathPoints, int i);

	UFUNCTION(meta = (DisplayName = "SetRunLocationUseDijkstra", Keywords = "OpenVRFramework SetRunLocationUseDijkstra"), Category = OpenVRFramework)
		void SetRunLocationUseDijkstraForAI(FVector HitLocation, FVector UnitLocation, TArray <AUnitBase*> Units, TArray<FPathPoint>& PathPoints, int i);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpacePressed", Keywords = "OpenVRFramework SpacePressed"), Category = OpenVRFramework)
		void SpacePressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpaceReleased", Keywords = "OpenVRFramework SpaceReleased"), Category = OpenVRFramework)
		void SpaceReleased();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void ToggleUnitDetection(AUnitBase* Unit);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "APressed", Keywords = "OpenVRFramework APressed"), Category = OpenVRFramework)
		void TPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AReleased", Keywords = "OpenVRFramework AReleased"), Category = OpenVRFramework)
		void AReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "JumpCamera", Keywords = "OpenVRFramework JumpCamera"), Category = OpenVRFramework)
		void JumpCamera();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StrgPressed", Keywords = "OpenVRFramework StrgPressed"), Category = OpenVRFramework)
		void StrgPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StrgReleased", Keywords = "OpenVRFramework StrgReleased"), Category = OpenVRFramework)
		void StrgReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomIn", Keywords = "OpenVRFramework ZoomIn"), Category = OpenVRFramework)
		void ZoomIn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOut", Keywords = "OpenVRFramework ZoomOut"), Category = OpenVRFramework)
		void ZoomOut();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		TSubclassOf<class AMissileRain> MissileRainClass;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SpawnMissileRain(int TeamId, FVector Location);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		TSubclassOf<class AEffectArea> EffectAreaClass;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SpawnEffectArea(int TeamId, FVector Location, FVector Scale, TSubclassOf<class AEffectArea> EAClass, AUnitBase* ActorToLockOn = nullptr);
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "IsShiftPressed", Keywords = "OpenVRFramework IsShiftPressed"), Category = OpenVRFramework)
		bool IsShiftPressed = false;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "AttackToggled", Keywords = "OpenVRFramework AttackToggled"), Category = OpenVRFramework)
		bool AttackToggled = false;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "IsStrgPressed", Keywords = "OpenVRFramework IsStrgPressed"), Category = OpenVRFramework)
		bool IsStrgPressed = false;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "IsSpacePressed", Keywords = "OpenVRFramework IsSpacePressed"), Category = OpenVRFramework)
		bool IsSpacePressed = false;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "AltIsPressed", Keywords = "OpenVRFramework AltIsPressed"), Category = OpenVRFramework)
		bool AltIsPressed = false;
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "LeftClickisPressed", Keywords = "OpenVRFramework LeftClickisPressed"), Category = OpenVRFramework)
		bool LeftClickIsPressed = false;
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "LockCameraToUnit", Keywords = "OpenVRFramework LockCameraToUnit"), Category = OpenVRFramework)
		bool LockCameraToUnit = false;
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "AIsPressed", Keywords = "OpenVRFramework AIsPressed"), Category = OpenVRFramework)
		int AIsPressedState = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "DIsPressed", Keywords = "OpenVRFramework DIsPressed"), Category = OpenVRFramework)
		int DIsPressedState = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "WIsPressed", Keywords = "OpenVRFramework WIsPressed"), Category = OpenVRFramework)
		int WIsPressedState = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "SIsPressed", Keywords = "OpenVRFramework SIsPressed"), Category = OpenVRFramework)
		int SIsPressedState = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
		bool MiddleMouseIsPressed = false;
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
		TArray <AUnitBase*> SelectedUnits;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		int SelectableTeamId = 0;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SetControlerTeamId(int Id);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SaveLevel(const FString& SlotName);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LoadLevel(const FString& SlotName);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LevelUp();
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void ResetTalents();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void HandleInvestment(int32 InvestmentState);


	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void SaveLevelUnit(const int32 UnitIndex, const FString& SlotName);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LoadLevelUnit(const int32 UnitIndex, const FString& SlotName);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void LevelUpUnit(const int32 UnitIndex);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void ResetTalentsUnit(const int32 UnitIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
		void HandleInvestmentUnit(const int32 UnitIndex, int32 InvestmentState);


	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SpendAbilityPoints(EGASAbilityInputID AbilityID, int Ability, const int32 UnitIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void ResetAbility(const int32 UnitIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void SaveAbility(const int32 UnitIndex, const FString& SlotName);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void LoadAbility(const int32 UnitIndex, const FString& SlotName);

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	int SelectedUnitCount = 0;


	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	float GetResource(int TeamId, EResourceType RType);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void ModifyResource(EResourceType ResourceType, int32 TeamId, float Amount);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void AddWorkerToResource(EResourceType ResourceType, int TeamId);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = OpenVRFramework)
	void RemoveWorkerFromResource(EResourceType ResourceType, int TeamId);
};

