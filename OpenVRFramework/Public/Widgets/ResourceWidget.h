// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ResourceWidget.generated.h"

/**
 * Widget class for displaying team resources in the game.
 */
UCLASS()
class OPENVRFRAMEWORK_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void UpdateTeamIdText();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetTeamId(int Id)
	{
		TeamId = Id;
		UpdateTeamIdText();
	}


	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void AddWorkerToResource(EResourceType ResourceType);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void RemoveWorkerFromResource(EResourceType ResourceType);

private:
	// Timer to track when to update the resource display
	float UpdateTimer = 0.0f;

	// Interval in seconds for how often to update the resource display
	const float UpdateInterval = 0.0f;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// Function to update the display of team resources on the widget
	void UpdateTeamResourcesDisplay();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	int32 TeamId;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TeamIdText;
	// TextFields for each type of resource
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* PrimaryResourceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* SecondaryResourceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* TertiaryResourceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* RareResourceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* EpicResourceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
	class UTextBlock* LegendaryResourceText;


	void UpdateWorkerCountDisplay();
	
    // Resource TextBlocks for displaying current worker counts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* PrimaryWorkerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* SecondaryWorkerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* TertiaryWorkerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* RareWorkerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* EpicWorkerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UTextBlock* LegendaryWorkerCount;

    // Buttons for adding workers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddPrimaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddSecondaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddTertiaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddRareWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddEpicWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* AddLegendaryWorkerButton;

    // Buttons for removing workers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemovePrimaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemoveSecondaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemoveTertiaryWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemoveRareWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemoveEpicWorkerButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = OpenVRFramework)
    UButton* RemoveLegendaryWorkerButton;
	
};