// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/UnitData.h"
#include "Kismet/GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "GameplayEffect.h"
#include "Pickup.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Type", Keywords = "Type CameraState"), Category = OpenVRFramework)
	TEnumAsByte<PickUpData::SelectableType> Type = PickUpData::Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int WeaponId; // Needed for MagazineCount

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float LifeTime = 0.f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxLifeTime = 0.f;
protected:
	// Called when the game starts or when spawneds
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Amount = 500.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> PickupEffect;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> PickupEffectTwo;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float DestructionDelayTime = 0.1f;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroySelectableWithDelay();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroySelectable();

	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void ImpactEvent();

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	int TeamId = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float PickUpDistance = 25.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool FollowTarget = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	AActor* Target;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MovementSpeed = 13.f;
};
