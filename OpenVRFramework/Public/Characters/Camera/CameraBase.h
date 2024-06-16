// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Controller/Input/InputConfig.h"
#include "InputActionValue.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Characters/Unit/UnitBase.h"
#include "Components/WidgetComponent.h"
#include "Core/UnitData.h"
#include "InputMappingContext.h"
#include "CameraBase.generated.h"


UCLASS()
class OPENVRFRAMEWORK_API ACameraBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//ACameraBase(const FObjectInitializer& ObjectInitializer);

	UCapsuleComponent* GetCameraBaseCapsule() const {
		return GetCapsuleComponent();
	}

	FVector PreviousMouseLocation;
	bool bLockCameraZRotation = true;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputConfig* InputConfig;

	bool BlockControls = true;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void SetActorBasicLocation();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "OpenVRFramework CreateCameraComp"), Category = OpenVRFramework)
		void CreateCameraComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RootScene", Keywords = "OpenVRFramework RootScene"), Category = OpenVRFramework)
		USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpringArm", Keywords = "OpenVRFramework SpringArm"), Category = OpenVRFramework)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpringArmRotator", Keywords = "OpenVRFramework SpringArmRotator"), Category = OpenVRFramework)
		FRotator SpringArmRotator = FRotator(-50, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraComp", Keywords = "OpenVRFramework CameraComp"), Category = OpenVRFramework)
		UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MappingContext", Keywords = "OpenVRFramework MappingContext"), Category = OpenVRFramework)
		UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MappingPriority", Keywords = "OpenVRFramework MappingPriority"), Category = OpenVRFramework)
		int32 MappingPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraDistanceToCharacter", Keywords = "OpenVRFramework CameraDistanceToCharacter"), Category = OpenVRFramework)
		float CameraDistanceToCharacter = 1500.f;

	UFUNCTION( BlueprintCallable, meta = (DisplayName = "PanMoveCamera", Keywords = "OpenVRFramework PanMoveCamera"), Category = OpenVRFramework)
		void PanMoveCamera(const FVector& NewPanDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Margin", Keywords = "OpenVRFramework Margin"), Category = OpenVRFramework)
		float Margin = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScreenSizeX", Keywords = "OpenVRFramework ScreenSizeX"), Category = OpenVRFramework)
		int32 ScreenSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScreenSizeY", Keywords = "OpenVRFramework ScreenSizeY"), Category = OpenVRFramework)
		int32 ScreenSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GetViewPortScreenSizesState", Keywords = "OpenVRFramework GetViewPortScreenSizesState"), Category = OpenVRFramework)
		int GetViewPortScreenSizesState = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		FVector CurrentCamSpeed = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float CamActorRespawnZLocation = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float ForceRespawnZLocation = -100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float AccelerationRate = 7500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float DecelerationRate = 15000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		float CamSpeed = 7000.f; // 120

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		float ZoomSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		float FastZoomSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
		float AutoZoomSpeed = 25.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float ZoomAccelerationRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float ZoomDecelerationRate = 15.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "EdgeScrollCamSpeed", Keywords = "OpenVRFramework EdgeScrollCamSpeed"), Category = OpenVRFramework)
		float EdgeScrollCamSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmMinRotator = -10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmMaxRotator = -50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmStartRotator = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmRotatorSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmRotatorMaxSpeed = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float SpringArmRotatorAcceleration = 0.05f;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void RotateSpringArm(bool Invert = false);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomIn", Keywords = "OpenVRFramework ZoomIn"), Category = OpenVRFramework)
		void ZoomIn(float ZoomMultiplier, bool Decelerate = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOut", Keywords = "OpenVRFramework ZoomOut"), Category = OpenVRFramework)
		void ZoomOut(float ZoomMultiplier, bool Decelerate = false);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void AdjustSpringArmRotation(float Difference, float& OutRotationValue);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void RotateSpringArmPitchFree(bool Invert);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void RotateSpringArmYawFree(bool Invert);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		bool RotateFree(FVector MouseLocation);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamLeft", Keywords = "OpenVRFramework RotateCamLeft"), Category = OpenVRFramework)
		bool RotateCamLeft(float Add, bool stopCam = false); // CamRotationOffset
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamRight", Keywords = "OpenVRFramework RotateCamRight"), Category = OpenVRFramework)
		bool RotateCamRight(float Add, bool stopCam = false); // CamRotationOffset
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamLeft", Keywords = "OpenVRFramework RotateCamLeft"), Category = OpenVRFramework)
		bool RotateCamLeftTo(float Position, float Add);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamRight", Keywords = "OpenVRFramework RotateCamRight"), Category = OpenVRFramework)
		bool RotateCamRightTo(float Position, float Add);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "JumpCamera", Keywords = "OpenVRFramework JumpCamera"), Category = OpenVRFramework)
		void JumpCamera(FHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		bool ZoomOutAutoCam(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOutToPosition", Keywords = "OpenVRFramework ZoomOutToPosition"), Category = OpenVRFramework)
		bool ZoomOutToPosition(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomInToPosition", Keywords = "OpenVRFramework ZoomInToPosition"), Category = OpenVRFramework)
		bool ZoomInToPosition(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockOnUnit", Keywords = "OpenVRFramework LockOnUnit"), Category = OpenVRFramework)
		void LockOnUnit(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		void LockOnActor(AActor* Actor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomOutPosition", Keywords = "OpenVRFramework ZoomOutPosition"), Category = OpenVRFramework)
		float ZoomOutPosition = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomPosition", Keywords = "OpenVRFramework ZoomPosition"), Category = OpenVRFramework)
		float ZoomPosition = 75.f; // 1500.f

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PitchValue", Keywords = "OpenVRFramework PitchValue"), Category = OpenVRFramework)
		float PitchValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "YawValue", Keywords = "OpenVRFramework YawValue"), Category = OpenVRFramework)
		float YawValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float CurrentRotationValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float RotationIncreaser = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		float RollValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool AutoLockOnSelect = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool DisableEdgeScrolling = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomThirdPersonPosition", Keywords = "OpenVRFramework ZoomThirdPersonPosition"), Category = OpenVRFramework)
		bool IsCharacterDistanceTooLow(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsCharacterDistanceTooHigh", Keywords = "OpenVRFramework IsCharacterDistanceTooHigh"), Category = OpenVRFramework)
		bool IsCharacterDistanceTooHigh(float Distance, const FVector SelectedActorPosition);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomThirdPersonPosition", Keywords = "OpenVRFramework ZoomThirdPersonPosition"), Category = OpenVRFramework)
		bool ZoomInToThirdPerson(const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomThirdPersonPosition", Keywords = "OpenVRFramework ZoomThirdPersonPosition"), Category = OpenVRFramework)
		float ZoomThirdPersonPosition = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamRotationOffset", Keywords = "OpenVRFramework CamRotationOffset"), Category = OpenVRFramework)
		float CamRotationOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AddCamRotation", Keywords = "OpenVRFramework AddCamRotation"), Category = OpenVRFramework)
		float AddCamRotation = 0.75f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "CameraAngles", Keywords = "OpenVRFramework CameraAngles"), Category = OpenVRFramework)
		float CameraAngles[4] = { 0.f, 90.f, 180.f, 270.f };

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RotationDegreeStep", Keywords = "OpenVRFramework RotationDegreeStep"), Category = OpenVRFramework)
		float RotationDegreeStep = 90.f;
	
	bool IsCameraInAngle()
	{
		if(SpringArmRotator.Yaw == 360.f) SpringArmRotator.Yaw = 0.f;
		bool IsInAngle = false;
		for( int i = 0; i < 4 ; i++)
		{
			if(SpringArmRotator.Yaw == CameraAngles[i]) IsInAngle = true;
		}
		return IsInAngle;
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToForward", Keywords = "OpenVRFramework MoveCamToForward"), Category = OpenVRFramework)
		void MoveCamToForward(float DeltaTime, bool Decelerate = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToBackward", Keywords = "OpenVRFramework MoveCamToBackward"), Category = OpenVRFramework)
		void MoveCamToBackward(float DeltaTime, bool Decelerate = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToLeft", Keywords = "OpenVRFramework MoveCamToLeft"), Category = OpenVRFramework)
		void MoveCamToLeft(float DeltaTime, bool Decelerate = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToRight", Keywords = "OpenVRFramework MoveCamToRight"), Category = OpenVRFramework)
		void MoveCamToRight(float DeltaTime, bool Decelerate = false);
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "StartTime", Keywords = "OpenVRFramework StartTime"), Category = OpenVRFramework)
		float StartTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraState", Keywords = "TopDownRTSTemplate CameraState"), Category = OpenVRFramework)
		TEnumAsByte<CameraData::CameraState> CameraState = CameraData::UseScreenEdges;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCameraState", Keywords = "OpenVRFramework SetCameraState"), Category = OpenVRFramework)
		void SetCameraState(TEnumAsByte<CameraData::CameraState> NewCameraState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCameraState", Keywords = "OpenVRFramework GetCameraState"), Category = OpenVRFramework)
		TEnumAsByte<CameraData::CameraState> GetCameraState();



	// Orbit
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OrbitCamLeft", Keywords = "OpenVRFramework OrbitCamLeft"), Category = OpenVRFramework)
	bool OrbitCamLeft(float Add);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector OrbitLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float OrbitRotationValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float OrbitIncreaser = 0.0001f; //0.0001f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float OrbitSpeed = 0.033f; //0.0010f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MovePositionCamSpeed = 1.0f;
	// Control Widget

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetComp", Keywords = "OpenVRFramework ControlWidgetComp"), Category = OpenVRFramework)
		class UWidgetComponent* ControlWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetRotator", Keywords = "OpenVRFramework ControlWidgetRotator"), Category = OpenVRFramework)
		FRotator ControlWidgetRotation = FRotator(50, 180, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetLocation", Keywords = "TopDownRTSTemplate ControlWidgetLocation"), Category = OpenVRFramework)
		FVector2D ControlWidgetLocation = FVector2D(0.5f, 0.5f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetHideLocation", Keywords = "OpenVRFramework ControlWidgetHideLocation"), Category = OpenVRFramework)
		FVector ControlWidgetHideLocation = FVector(400.f, -2500.0f, -250.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		bool ShowControlWidgetAtStart = true;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HideControlWidget", Keywords = "OpenVRFramework HideControlWidget"), Category = OpenVRFramework)
		void HideControlWidget();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShowControlWidget", Keywords = "OpenVRFramework ShowControlWidget"), Category = OpenVRFramework)
		void ShowControlWidget();

	//UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
		//void SetControlWidgetLocation();
	
	/////// Loading Widget ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoadingWidgetComp", Keywords = "OpenVRFramework LoadingWidgetComp"), Category = OpenVRFramework)
	class UWidgetComponent* LoadingWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoadingWidgetRotation", Keywords = "OpenVRFramework LoadingWidgetRotation"), Category = OpenVRFramework)
	FRotator LoadingWidgetRotation = FRotator(50.f, 180, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoadingWidgetLocation", Keywords = "OpenVRFramework LoadingWidgetLocation"), Category = OpenVRFramework)
	FVector LoadingWidgetLocation = FVector(150.f, 000.0f, -150.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoadingWidgetHideLocation", Keywords = "OpenVRFramework LoadingWidgetHideLocation"), Category = OpenVRFramework)
	FVector LoadingWidgetHideLocation = FVector(300.f, -2200.0f, -250.0f);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HideLoadingWidget", Keywords = "OpenVRFramework HideLoadingWidget"), Category = OpenVRFramework)
		void DeSpawnLoadingWidget();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnLoadingWidget", Keywords = "OpenVRFramework SpawnLoadingWidget"), Category = OpenVRFramework)
		void SpawnLoadingWidget();
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


};
