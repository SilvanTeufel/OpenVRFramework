// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Unit/UnitBase.h"
#include "HealingActor.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AHealingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingActor();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "OpenVRFramework TriggerCapsule"), Category = OpenVRFramework)
	class UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UMaterialInterface* Material;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Init(AUnitBase* Target, AUnitBase* Healer);

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	AUnitBase* HealingUnit;
	
	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	TArray<AUnitBase*> Actors;

	UPROPERTY( BlueprintReadWrite, Category = OpenVRFramework)
	float ControlTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float HealIntervalTime = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	int ControlInterval = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int MaxIntervals = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float IntervalHeal = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MainHeal = 25.f;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
