// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Core/Talents.h"
#include "Characters/Unit/LevelUnit.h"
#include "TalentChooser.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API UTalentChooser : public UUserWidget
{
	GENERATED_BODY()
    
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void UpdateProgressBars();
	
	TArray<FGameplayAttributeData*> Attributes;

	// Function to initialize the Attributes array
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void InitializeAttributes();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<FString> AttributeNames = {"Stamina", "Attack Power", "Willpower", "Haste", "Armor", "MagicResistance"};

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	ALevelUnit* GetOwnerActor() {
		return OwnerUnitBase;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	ALevelUnit* OwnerUnitBase;

	// Dynamic arrays for UI elements
	UPROPERTY(meta = (BindWidget))
	TArray<class UProgressBar*> ClassProgressBars;

	UPROPERTY(meta = (BindWidget))
	TArray<class UButton*> ClassButtons;

	UPROPERTY(meta = (BindWidget))
	TArray<class UTextBlock*> ClassNames;

	// Functions to dynamically create UI elements
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void UpdateExperience();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void UpdateLevelAndTalents();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void CreateClassUIElements();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void InitializeLevelAndTalentUI();

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExperienceProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock*  CurrentLevel;

	UPROPERTY(meta = (BindWidget))
	class UButton* LevelUpButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock*  AvailableTalents;

	UPROPERTY(meta = (BindWidget))
	class UButton*  ResetTalentsButton;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void HandleTalentButtonClicked(int32 ButtonIndex);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnLevelUpClicked();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnResetTalentsClicked();
	
public:
	virtual void NativeConstruct() override;

	void SetOwnerActor(ALevelUnit* Unit) {
		OwnerUnitBase = Unit;
	}

	// Additional helper functions as needed
	// ...
};
