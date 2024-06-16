// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API AEOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnNetCleanup(UNetConnection* Connection) override;
	
};
