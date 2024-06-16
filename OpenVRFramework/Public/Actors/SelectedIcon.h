// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SelectedIcon.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API ASelectedIcon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelectedIcon();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	UStaticMeshComponent* IconMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	UMaterialInterface* BlueMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	UMaterialInterface* ActionMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	UMaterialInstanceDynamic* DynMaterial;

	// Global string definitions
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FString SphereMeshAssetPath = TEXT("/Engine/BasicShapes/Plane");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FString MaterialBluePath = TEXT("Material'/OpenVRFramework/OpenVRFramework/Materials/M_Ring_Aura.M_Ring_Aura'");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FString MaterialActionPath = TEXT("Material'/OpenVRFramework/OpenVRFramework/Materials/M_Ring_Aura_Red.M_Ring_Aura_Red'");

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = OpenVRFramework)
	void ChangeMaterialColour(FVector4d Colour);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void ChangeMaterialToAction();

	float TriggerCapsuleRadius = 300.f;
	float TriggerCapsuleHeight = 300.f;

	UFUNCTION(BlueprintImplementableEvent, Category = OpenVRFramework)
	void ImpactEvent();
};
