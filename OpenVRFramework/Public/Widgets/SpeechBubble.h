// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Input/SButton.h"
#include "SpeechButton.h"
#include "Core/UnitData.h"
#include "Components/AudioComponent.h"
#include "SpeechBubble.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchWidgetDelegate, int, Index);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetTableDelegate, int, New_Text_Id);
/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API USpeechBubble : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Button_Text_1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Button_Text_2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Button_Text_3;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Button_Text_4;
	
	UPROPERTY(meta = (BindWidget))
	class USpeechButton* Button_1;
	
	UPROPERTY(meta = (BindWidget))
	class USpeechButton* Button_2;

	UPROPERTY(meta = (BindWidget))
	class USpeechButton* Button_3;
	
	UPROPERTY(meta = (BindWidget))
	class USpeechButton* Button_4;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchWidgetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseWidgetButton;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void OnClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	int WidgetIndex = 0;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SwitchWidget(int Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	FSwitchWidgetDelegate CallbackSwitchWidgetDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* SpeechSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UAudioComponent* SpeechSoundComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float SpeechSoundTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	USoundBase* BackgroundSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	UAudioComponent* BackgroundSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BackgroundSoundTimer = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class USpeechButton* SpeechButtonWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	TSubclassOf<USpeechButton> SpeechButtonWidgetClass;

	int Text_Id = 1;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetTableDataId(int T_Id);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void PlayButtonSound(int B_Id);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class UDataTable* SpeechDataTable_Texts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	class UDataTable* SpeechDataTable_Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float BlendPoint_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float MaxAnimationTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenVRFramework)
	float AnimationTime;
};
