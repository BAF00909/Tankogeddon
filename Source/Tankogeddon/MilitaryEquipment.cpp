// Fill out your copyright notice in the Description page of Project Settings.


#include "MilitaryEquipment.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "GameStructs.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Tankogeddon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"

// Sets default values
AMilitaryEquipment::AMilitaryEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->onDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->onDamaged.AddUObject(this, &ATankPawn::DamageTaked);

}

// Called when the game starts or when spawned
void AMilitaryEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMilitaryEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMilitaryEquipment::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TankDamage(DamageData);
}

void AMilitaryEquipment::Die()
{
	Destroy();
}

void AMilitaryEquipment::DamageTaked(float DamageValue)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

