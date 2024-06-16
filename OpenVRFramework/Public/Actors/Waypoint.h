// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Waypoint.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AWaypoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaypoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FString Tag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	AWaypoint* NextWaypoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	AUnitBase* ActualCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool PatrolCloseToWaypoint = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector2D PatrolCloseOffset = FVector2D(1000.f, 1000.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float PatrolCloseMinInterval = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float PatrolCloseMaxInterval = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float RandomTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float PatrolCloseIdlePercentage = 40.0f;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FTimerHandle TimerHandle;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = OpenVRFramework)
	void TimerFunction();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = OpenVRFramework)
	void SetupTimerFunction();
	*/
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = OpenVRFramework)
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
