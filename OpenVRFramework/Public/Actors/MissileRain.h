// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Missile.h"
#include "MissileRain.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AMissileRain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissileRain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;
	
	UFUNCTION()
	void SpawnMissile(int TeamId, FVector SpawnLocation, float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector ScaleMissile = FVector(0.1f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int MissileCount= 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int TeamId= 1.f;
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxLifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float RainTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MissileRange = 500.f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	//float SpawnRainTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float SpawnMissileTime = 0.025f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int SpawnCounter = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<class AMissile> MissileBaseClass;
};
