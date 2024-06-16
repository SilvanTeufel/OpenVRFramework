// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SessionItem.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API USessionItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = OpenVRFramework)
	UTextBlock* MyTextBlock;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetItemName(FText NewItemName);
};
