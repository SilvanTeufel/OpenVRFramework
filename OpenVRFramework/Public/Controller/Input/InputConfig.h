// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
//#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "InputConfig.generated.h"

class UInputAction;
struct FGameplayTag;


USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "InputAction", Keywords = "OpenVRFramework InputAction"), Category = OpenVRFramework)
	const UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "InputTag", Keywords = "OpenVRFramework InputTag"), Category = OpenVRFramework)
	FGameplayTag InputTag;
};

/**
 *
 */
UCLASS()
class OPENVRFRAMEWORK_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Returns the first Input Action associated with a given tag.
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

public:
	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "TaggedInputActions", Keywords = "OpenVRFramework TaggedInputActions"), Category = OpenVRFramework)
	TArray<FTaggedInputAction> TaggedInputActions;
};