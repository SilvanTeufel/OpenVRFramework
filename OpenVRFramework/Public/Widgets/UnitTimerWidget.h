// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Unit/UnitBase.h"
#include "UnitTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UUnitTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
public:
	void SetOwnerActor(AUnitBase* Unit) {
		OwnerCharacter = Unit;
	}


protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<AUnitBase> OwnerCharacter;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* TimerBar;

	bool IsVisible = false;
	bool SetVisible = false;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FLinearColor CastingColor = FLinearColor::Red; // Replace with your desired color

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FLinearColor PauseColor = FLinearColor::Yellow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FLinearColor BuildColor = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FLinearColor ExtractionColor = FLinearColor::Black;
};
