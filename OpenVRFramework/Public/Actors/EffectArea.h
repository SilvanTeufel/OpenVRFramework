// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Unit/UnitBase.h"
#include "TimerManager.h"
#include "EffectArea.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AEffectArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectArea();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FTimerHandle DamageTimerHandle;

	FTimerHandle ScaleTimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void ScaleMesh();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetScaleTimer();
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxLifeTime = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int TeamId = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsGettingBigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BiggerScaler = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BiggerScaleInterval = 1.0f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> AreaEffectOne;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> AreaEffectTwo;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> AreaEffectThree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsHealing = false;

	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void ImpactEvent(AUnitBase* Unit);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
