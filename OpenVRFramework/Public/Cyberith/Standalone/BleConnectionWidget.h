#pragma once
#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "BleConnectionWidget.generated.h"

DECLARE_DELEGATE(OnNativeConstruct);

UCLASS(Abstract)
class OPENVRFRAMEWORK_API UBleConnectionWidget: public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	OnNativeConstruct NativeConstructDelegate;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UCanvasPanel* SelectingObjectsHolder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UTextBlock* SelectingText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UScrollBox* SelectingButtonsScrollBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UTextBlock* ConnectionText;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UTextBlock* StatusText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UCanvasPanel* ConnectionObjectsHolder;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UCanvasPanel* FailedObjectsHolder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UTextBlock* FailedText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category = "Virtualizer|Standalone")
	UButton* ReconnectButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional), Category = "Virtualizer|Standalone")
	UCanvasPanel* CalibrationObjectsHolder;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional), Category = "Virtualizer|Standalone")
	UTextBlock* CalibrationText;

	UPROPERTY()
	TArray<UCanvasPanel*> ObjectsHolders;
};
