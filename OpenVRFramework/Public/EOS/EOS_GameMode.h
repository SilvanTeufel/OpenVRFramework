// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/ResourceGameMode.h"
#include "EOS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AEOS_GameMode : public AResourceGameMode
{
	GENERATED_BODY()

//private:
	//UFUNCTION()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void Register(APlayerController* NewPlayer);

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	FUniqueNetIdRepl UniqueNetIdRepl;
	
};
