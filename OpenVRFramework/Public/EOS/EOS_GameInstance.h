// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h" 
#include "EOS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENVRFRAMEWORK_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TSharedPtr<const FUniqueNetId> StoredUserId;
public:
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = OpenVRFramework)
	FString GetPlayerUserName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = OpenVRFramework)
	bool IsPlayerLoggedIn();
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void FindSessionAndJoin();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	bool SessionJoined = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	bool SessionCreationExecuted = false;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	//bool loading = false;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void DestroySession();
	
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(BlueprintReadWrite, Category = OpenVRFramework)
	TArray<FString> SearchNames;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FName RTSSessionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenVRFramework)
	FName RTSSessionSearchKey;
	
	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void SetRTSSessionName(const FText& NewSessionName)
	{
		RTSSessionName = FName(*NewSessionName.ToString());
		RTSSessionSearchKey = FName(*NewSessionName.ToString());
		// Log the new RTSSessionName value
		UE_LOG(LogTemp, Log, TEXT("RTSSessionName set to: %s"), *RTSSessionName.ToString());
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EOSVariables")
	FString OpenLevelText;

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void CreateEosSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category = OpenVRFramework)
	void QuitSession();
	
	void LoginWithEos_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	void OnFindSessionCompleted(bool bWasSucces);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccesful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccesful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
