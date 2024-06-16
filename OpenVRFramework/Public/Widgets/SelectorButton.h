// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SelectorButton.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetButtonIdDelegate, int, New_Text_Id);

UCLASS()
class OPENVRFRAMEWORK_API USelectorButton : public UButton
{
	GENERATED_BODY()

	
public:
	USelectorButton();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int Id = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FSetButtonIdDelegate CallbackIdDelegate;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnClick();

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetUnitSelectorId(int newID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UUserWidget* Selector;

	bool SelectUnit = false;
};
