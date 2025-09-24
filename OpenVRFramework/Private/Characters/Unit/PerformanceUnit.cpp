// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Characters/Unit/PerformanceUnit.h"

#include "Net/UnrealNetwork.h"
#include "Widgets/UnitBaseHealthBar.h"
#include "Widgets/UnitTimerWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Controller/AIController/UnitControllerBase.h"
#include "Engine/SkeletalMesh.h" // For USkeletalMesh
#include "Engine/SkeletalMeshLODSettings.h" // To access LOD settings
#include "Engine/SkinnedAssetCommon.h"

APerformanceUnit::APerformanceUnit(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

void APerformanceUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	CheckViewport();
	CheckTeamVisibility();
	
	if(EnableFog)VisibilityTickFog();
	else SetCharacterVisibility(IsOnViewport);
	
	CheckHealthBarVisibility();
	CheckTimerVisibility();
	*/
}

void APerformanceUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APerformanceUnit, OpenHealthWidget);
	DOREPLIFETIME(APerformanceUnit, HealthWidgetComp);
	DOREPLIFETIME(APerformanceUnit, TimerWidgetComp);
	DOREPLIFETIME(APerformanceUnit, HealthWidgetCompLocation);
}

void APerformanceUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void APerformanceUnit::Destroyed()
{
	Super::Destroyed();
	
}

void APerformanceUnit::SetCharacterVisibility(bool desiredVisibility)
{
		USkeletalMeshComponent* SkelMesh = GetMesh();
		if (SkelMesh)
		{
			SkelMesh->SetVisibility(desiredVisibility, true);
			SkelMesh->bPauseAnims = !desiredVisibility;
		}
}


void APerformanceUnit::VisibilityTickFog()
{
	
	if (IsMyTeam)
	{
		SetCharacterVisibility(IsOnViewport);
	}else
	{
		if(IsOnViewport)SetCharacterVisibility(IsVisibileEnemy);
		else SetCharacterVisibility(false);
	}

}

void APerformanceUnit::CheckViewport()
{
	if (IsInViewport(GetActorLocation(), VisibilityOffset))
	{
		IsOnViewport = true;
	}
	else
	{
		IsOnViewport = false;
	}
}


bool APerformanceUnit::IsInViewport(FVector WorldPosition, float Offset)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FVector2D ScreenPosition;
		UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition);

		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		return ScreenPosition.X >= -Offset && ScreenPosition.X <= ViewportSizeX + Offset && ScreenPosition.Y >= -Offset && ScreenPosition.Y <= ViewportSizeY + Offset;
	}

	return false;
}

void APerformanceUnit::CheckHealthBarVisibility()
{
	if(HealthWidgetComp)
	if (UUnitBaseHealthBar* HealthBarWidget = Cast<UUnitBaseHealthBar>(HealthWidgetComp->GetUserWidgetObject()))
	{
		if (IsOnViewport && OpenHealthWidget && !HealthBarUpdateTriggered && (IsVisibileEnemy || IsMyTeam))
		{
			HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
			HealthBarUpdateTriggered = true;
			if(Projectile) Projectile->SetVisibility(true);
		}
		else if(HealthBarUpdateTriggered && !OpenHealthWidget)
		{
			if(Projectile) Projectile->SetVisibility(false);
			HealthBarWidget->SetVisibility(ESlateVisibility::Collapsed);
			HealthBarUpdateTriggered = false;
		}

		if(HealthBarUpdateTriggered)	
			HealthBarWidget->UpdateWidget();
	}
}

void APerformanceUnit::CheckTimerVisibility()
{
	if (UUnitTimerWidget* TimerWidget = Cast<UUnitTimerWidget>(TimerWidgetComp->GetUserWidgetObject())) // Assuming you have a UUnitBaseTimer class for the timer widget
	{
		if (IsOnViewport && IsMyTeam)
		{
			TimerWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			TimerWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}