// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller/ControllerBase.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "Controller/Input/EnhancedInputComponentBase.h"
#include "CameraControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API ACameraControllerBase : public AControllerBase
{
	GENERATED_BODY()
	
	ACameraControllerBase();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void MoveCamToLocation(ACameraBase* Camera, const FVector& DestinationLocation);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		bool CheckSpeakingUnits();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetCameraState(TEnumAsByte<CameraData::CameraState> NewCameraState);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GetViewPortScreenSizes(int x);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FVector GetCameraPanDirection();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	FVector CalculateUnitsAverage(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GetAutoCamWaypoints();
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	int UnitCountInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int UnitCountToZoomOut = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int UnitZoomScaler = 10;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetCameraAveragePosition(ACameraBase* Camera, float DeltaTime);
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	float CalcControlTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float OrbitAndMovePauseTime = 5.f;
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	float OrbitLocationControlTimer;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	int OrbitRotatorIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray <FVector> OrbitPositions = { FVector(0.f), FVector(3000.f, 3000.f, 0.f) };
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = OpenVRFramework)
	TArray <float> OrbitRadiuses = { 3000.f, 1000.f};
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = OpenVRFramework)
	TArray <float> OrbitTimes = { 5.f, 5.f};

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = OpenVRFramework)
	float UnitCountOrbitTimeMultiplyer = 0.5f;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void CameraBaseMachine(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void RotateCam(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void MoveCamToPosition(float DeltaSeconds, FVector Destination);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void MoveCamToClick(float DeltaSeconds, FVector Destination);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void MoveCam(float DeltaSeconds, FVector Destination);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OrbitAtLocation(FVector Destination, float OrbitSpeed);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleLockCamToCharacter", Keywords = "OpenVRFramework ToggleLockCamToCharacter"), Category = OpenVRFramework)
	void ToggleLockCamToCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnlockCamFromCharacter", Keywords = "OpenVRFramework UnlockCamFromCharacter"), Category = OpenVRFramework)
	void UnlockCamFromCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToSpecificUnit", Keywords = "OpenVRFramework LockCamToSpecificUnit"), Category = OpenVRFramework)
	void LockCamToSpecificUnit(AUnitBase* SUnit);

	ASpeakingUnit* SpeakingUnit;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "OpenVRFramework LockCamToCharacter"), Category = OpenVRFramework)
	void LockCamToCharacter(int Index);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "OpenVRFramework LockCamToCharacter"), Category = OpenVRFramework)
	void LockZDistanceToCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCameraZDistance", Keywords = "OpenVRFramework SetCameraZDistance"), Category = OpenVRFramework)
	void SetCameraZDistance(int Index);
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LockCameraToCharacter", Keywords = "OpenVRFramework LockCameraToCharacter"), Category = OpenVRFramework)
	bool LockCameraToCharacter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool RotateBehindCharacterIfLocked = true;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingRight", Keywords = "OpenVRFramework CamIsRotatingRight"), Category = OpenVRFramework)
	bool CamIsRotatingRight = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingLeft", Keywords = "OpenVRFramework CamIsRotatingLeft"), Category = OpenVRFramework)
	bool CamIsRotatingLeft = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingInState", Keywords = "OpenVRFramework CamIsZoomingInState"), Category = OpenVRFramework)
	int CamIsZoomingInState = 0;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingOutState", Keywords = "OpenVRFramework CamIsZoomingOutState"), Category = OpenVRFramework)
	int CamIsZoomingOutState = 0;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomOutToPosition", Keywords = "OpenVRFramework ZoomOutToPosition"), Category = OpenVRFramework)
	bool ZoomOutToPosition = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomInToPosition", Keywords = "OpenVRFramework ZoomInToPosition"), Category = OpenVRFramework)
	bool ZoomInToPosition = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool AutoCamPlayerOnly = true;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "HoldZoomOnLockedCharacter", Keywords = "OpenVRFramework HoldZoomOnLockedCharacter"), Category = OpenVRFramework)
	bool HoldZoomOnLockedCharacter = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScrollZoomCount", Keywords = "OpenVRFramework ScrollZoomCount"), Category = OpenVRFramework)
	float ScrollZoomCount = 0.f;

};
