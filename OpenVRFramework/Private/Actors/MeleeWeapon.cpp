// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MeleeWeapon.h"

#include "Characters/Unit/UnitBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
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

void AMeleeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMeleeWeapon, Mesh);
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeapon::Init(AActor* NewCarrier, int WeaponTeamId)
{
	UE_LOG(LogTemp, Warning, TEXT("INitialise Carrier!!!"));
	Carrier = NewCarrier;
	TeamId = WeaponTeamId;
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
}

void AMeleeWeapon::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin_Implementation!"));
	if(OtherActor)
	{
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);
		if(UnitToHit && UnitToHit->GetUnitState() == UnitData::Dead)
		{
			ImpactEvent();

		}if(UnitToHit && UnitToHit->TeamId != TeamId)
		{
			ImpactEvent();
			Impact(OtherActor);
			//SetIsAttacked(UnitToHit);
			//DestroyWhenMaxPierced();
		}else if(UnitToHit && UnitToHit->TeamId == TeamId)
		{
			ImpactEvent();
			//ImpactHeal(Target);
			//DestroyWhenMaxPierced();
		}
			
	}
}


void AMeleeWeapon::Impact_Implementation(AActor* ImpactTarget)
{
	AUnitBase* CurrentCarrier = Cast<AUnitBase>(Carrier);
	AUnitBase* UnitToHit = Cast<AUnitBase>(ImpactTarget);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile ShootingUnit->Attributes->GetAttackDamage()! %f"), ShootingUnit->Attributes->GetAttackDamage());
	if(UnitToHit && UnitToHit->TeamId != TeamId && CurrentCarrier)
	{
		float NewDamage = CurrentCarrier->Attributes->GetAttackDamage() - UnitToHit->Attributes->GetArmor();
			
		if(CurrentCarrier && CurrentCarrier->IsDoingMagicDamage)
			NewDamage = CurrentCarrier->Attributes->GetAttackDamage() - UnitToHit->Attributes->GetMagicResistance();
			
		if(UnitToHit->Attributes->GetShield() <= 0)
			UnitToHit->SetHealth(UnitToHit->Attributes->GetHealth()-NewDamage);
		else
			UnitToHit->Attributes->SetAttributeShield(UnitToHit->Attributes->GetShield()-NewDamage);


		if(UnitToHit && UnitToHit->GetUnitState() == UnitData::Dead)
		{
			// Do Nothing
		}else if(UnitToHit && UnitToHit->TeamId != TeamId)
		{
			UnitToHit->ApplyInvestmentEffect(MeeleEffect);
		}
		
		CurrentCarrier->LevelData.Experience++;
		UnitToHit->ActivateAbilityByInputID(UnitToHit->DefensiveAbilityID, UnitToHit->DefensiveAbilities);
		if(UnitToHit->GetUnitState() != UnitData::Dead &&
			UnitToHit->GetUnitState() != UnitData::IsAttacked )
		{
			UnitToHit->UnitControlTimer = 0.f;
			UnitToHit->SetUnitState(UnitData::IsAttacked);
		}
	}	
}