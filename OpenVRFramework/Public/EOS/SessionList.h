// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "EOS/SessionItem.h"
#include "SessionList.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API USessionList : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = OpenVRFramework)
	UListView* MyListView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<UUserWidget> ListItemClass;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateListView();

private:
	FTimerHandle UpdateTimerHandle;
};
