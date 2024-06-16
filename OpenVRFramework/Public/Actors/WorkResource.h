// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Characters/Unit/UnitBase.h"
#include "TimerManager.h"
#include "Core/WorkerData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "WorkResource.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API AWorkResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorkResource();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class UCapsuleComponent* TriggerCapsule;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void ImpactEvent();
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	float PickUpDistance = 25.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool FollowTarget = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = OpenVRFramework)
	AActor* Target;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MovementSpeed = 13.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	bool IsAttached = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	EResourceType ResourceType = EResourceType::Primary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Amount = 10.f;

	FVector SocketOffset = FVector(10.0f, 10.0f, 0.0f);
};
