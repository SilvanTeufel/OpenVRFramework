// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MeeleWeapon.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMeeleWeapon::AMeeleWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
    
	// Configure collision settings
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName(TEXT("Trigger")); // Set collision profile
	Mesh->SetGenerateOverlapEvents(true);


	if (HasAuthority())
	{
		bReplicates = true;
		SetReplicateMovement(true);
	}
}

void AMeeleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMeeleWeapon, Mesh);
}

// Called when the game starts or when spawned
void AMeeleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeeleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

