// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/UnitData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "UnitBaseAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FUnitAnimData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TEnumAsByte<UnitData::EState> AnimState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float TransitionRate_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float TransitionRate_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Resolution_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Resolution_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* Sound;
	
};

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class OPENVRFRAMEWORK_API UUnitBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UUnitBaseAnimInstance();
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		TEnumAsByte<UnitData::EState> CharAnimState;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
		TEnumAsByte<UnitData::EState> LastAnimState = UnitData::None;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_1 = 0;
	
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_2 = 0;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float CurrentBlendPoint_1 = 0;
	
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float CurrentBlendPoint_2 = 0;
	
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float TransitionRate_1 = 0.5;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float TransitionRate_2 = 0.5;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Resolution_1 = 0;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float Resolution_2 = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* Sound;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float SoundTimer = 0.f;
	
	UFUNCTION()
	virtual void NativeInitializeAnimation() override;
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class UDataTable* AnimDataTable;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetBlendPoints(AUnitBase* Unit, float Deltaseconds);
	
	FUnitAnimData* UnitAnimData;
	
};