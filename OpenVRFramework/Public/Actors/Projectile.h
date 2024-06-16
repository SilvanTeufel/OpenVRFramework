// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Projectile.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void Init(AActor* TargetActor, AActor* ShootingActor);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void InitForAbility(AActor* TargetActor, AActor* ShootingActor);

	
	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "Target", Keywords = "OpenVRFramework Target"), Category = OpenVRFramework)
	AActor* Target;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (DisplayName = "Target", Keywords = "OpenVRFramework Target"), Category = OpenVRFramework)
	AActor* Shooter;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	FVector TargetLocation;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	FVector ShooterLocation;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool FollowTarget = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int MaxPiercedTargets = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,  Category = OpenVRFramework)
	int PiercedTargets = 0;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool RotateMesh = false;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FVector RotationSpeed = FVector(0.5f);
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UMaterialInterface* Material;

	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite, Category = OpenVRFramework)
	float Damage;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float LifeTime = 0.f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxLifeTime = 2.f;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	int TeamId = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MovementSpeed = 50.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UGameplayEffect> ProjectileEffect;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsHealing = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsBouncingBack = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsBouncingNext = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	bool BouncedBack = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Impact(AActor* ImpactTarget);

	UFUNCTION(Server, Reliable)
	void ImpactHeal(AActor* ImpactTarget);
	
	UFUNCTION(Server, Reliable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void ImpactEvent();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroyWhenMaxPierced();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroyProjectileWithDelay();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroyProjectile();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetIsAttacked(AUnitBase* UnitToHit);
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float DestructionDelayTime = 0.1f;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetBackBouncing(AUnitBase* ShootingUnit);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetNextBouncing(AUnitBase* ShootingUnit, AUnitBase* UnitToHit);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	AUnitBase* GetNextUnitInRange(AUnitBase* ShootingUnit, AUnitBase* UnitToHit);
};
