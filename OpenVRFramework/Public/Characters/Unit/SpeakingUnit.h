// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Widgets/SpeechBubble.h"
#include "Components/AudioComponent.h"
#include "SpeakingUnit.generated.h"

UCLASS()
class OPENVRFRAMEWORK_API ASpeakingUnit : public AUnitBase
{
	GENERATED_BODY()
public:
	ASpeakingUnit(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	bool IsResponsive = true;
	virtual void Tick(float DeltaTime) override;

	void PlaySoundOnce(float Timer, UAudioComponent*& Component, USoundBase* Sound, float Volume);
	
	void SetSpeechWidgetText();

	void SetSpeechWidgetLocation(FVector NewLocation);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechMarkerWidgetComp", Keywords = "OpenVRFramework SpeechMarkerWidgetComp"), Category = OpenVRFramework)
	class UWidgetComponent* SpeechMarkerWidgetComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubbleWidgetComp", Keywords = "OpenVRFramework SpeechBubbleWidgetComp"), Category = OpenVRFramework)
	class UWidgetComponent* SpeechBubbleWidgetComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubbleWidgetCompLocation", Keywords = "OpenVRFramework SpeechBubbleWidgetCompLocation"), Category = OpenVRFramework)
	FVector SpeechBubbleWidgetCompLocation = FVector (0.f, 0.f, 220.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubble", Keywords = "OpenVRFramework SpeechBubble"), Category = OpenVRFramework)
	USpeechBubble* SpeechBubble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LockCamOnUnit", Keywords = "OpenVRFramework LockCamOnUnit"), Category = OpenVRFramework)
	bool LockCamOnUnit = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeakZoomPosition", Keywords = "OpenVRFramework SpeakZoomPosition"), Category = OpenVRFramework)
	float SpeakZoomPosition = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechVolume", Keywords = "OpenVRFramework SpeechVolume"), Category = OpenVRFramework)
	float SpeechVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "BackgroundVolume", Keywords = "OpenVRFramework BackgroundVolume"), Category = OpenVRFramework)
	float BackgroundVolume = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MinSpeakDistance", Keywords = "OpenVRFramework MinSpeakDistance"), Category = OpenVRFramework)
	float MinSpeakDistance = 500.f;
	
	int Text_Id = 1;
	

	
};
