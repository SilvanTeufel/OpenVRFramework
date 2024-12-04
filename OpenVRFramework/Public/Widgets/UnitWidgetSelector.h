// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectorButton.h"
#include "Components/TextBlock.h"
#include "Controller/PlayerController/ControllerBase.h"
#include "UnitWidgetSelector.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UUnitWidgetSelector : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<class USelectorButton*> SingleSelectButtons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<class USelectorButton*> SelectButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TArray<class UTextBlock*> ButtonLabels;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void ChangeButtonColor(UButton* Button, FLinearColor NewColor);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void GetButtonsFromBP();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetButtonColours(int AIndex);
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetButtonIds();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetVisibleButtonCount(int32 Count);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetButtonLabelCount(int32 Count);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int ShowButtonCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int MaxButtonCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	AControllerBase* ControllerBase;
	
};
