// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CalibrationManager.generated.h"

UENUM(BlueprintType)
enum ECalibrationDataResult {
	Success = 1,
	Exception = 0,
	NoCalibrationApp = -1,
	NoCalibrationData = -2,
	BoundaryNotConfigured = -3,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENVRFRAMEWORK_API UCalibrationManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCalibrationManager();

	UFUNCTION(BlueprintCallable, Category = "Virtualizer|Standalone")
	ECalibrationDataResult GetCalibrationDataResult() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CalculateTransform(float boundaryPoints[2][2]);

	void LoadCalibrationData();

	virtual void OnRecentered() {};

	ECalibrationDataResult m_calibrationDataResult;
	
private:
	struct hMatrix {
		float m[2][3];
	};
	struct rMatrix {
		float m[2][2];
	};
	struct tVector {
		float v[2];
	};

	hMatrix GetHomogeneousTransformationMatrix(float boundaryPoints[2][2]) const;
	rMatrix GetRotationMatrix(float boundaryPoints[2][2]) const;
	tVector GetTranslationVector(float boundaryPoints[2][2], rMatrix R) const;

	float m_calibratedPositionX;
	float m_calibratedPositionY;
	float m_calibratedRotationZ;
	float m_boundary0X;
	float m_boundary0Y;
	float m_boundary1X;
	float m_boundary1Y;

	
	UPROPERTY()
	USceneComponent* m_VrOriginComponent;
};
