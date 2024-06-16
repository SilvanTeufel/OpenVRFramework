// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GAS/AttributeSetBase.h"
#include "Core/UnitData.h"
#include "Core/Talents.h"
#include "GAS/GAS.h"
#include "TalentSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UTalentSaveGame : public USaveGame
{
	GENERATED_BODY()

	

public:
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void PopulateAttributeSaveData(UAttributeSetBase* AttributeSet);
	
	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	FLevelData LevelData;

	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	FLevelUpData LevelUpData;
	
	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	FAttributeSaveData AttributeSaveData;

	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	EGASAbilityInputID OffensiveAbilityID;

	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	EGASAbilityInputID DefensiveAbilityID;

	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	EGASAbilityInputID AttackAbilityID;

	UPROPERTY(VisibleAnywhere, Category = OpenVRFramework)
	EGASAbilityInputID ThrowAbilityID;
};
