// Fill out your copyright notice in the Description page of Project Settings.


#include "MilitaryEquipment.h"
#include <Components/StaticMeshComponent.h>
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Tankogeddon.h"
#include "HealthComponent.h"
#include "Cannon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMilitaryEquipment::AMilitaryEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body mesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->onDie.AddUObject(this, &AMilitaryEquipment::Die);
	HealthComponent->onDamaged.AddUObject(this, &AMilitaryEquipment::DamageTaked);
}

void AMilitaryEquipment::BeginPlay()
{
	Super::BeginPlay();

}

void AMilitaryEquipment::Die()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestuctionParticleSystem, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
	Destroy();
}

void AMilitaryEquipment::TakeDamage(FDamageData DamageData)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Turret %s take damage: %f"), *GetName(), DamageData.DamageValue);
	HealthComponent->TankDamage(DamageData);
}

void AMilitaryEquipment::DamageTaked(float DamageValue)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

FVector AMilitaryEquipment::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

