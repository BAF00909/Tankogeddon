// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "MilitaryEquipment.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	//RootComponent = BodyMesh;

	/*TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);*/

	/*HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->onDie.AddUObject(this, &ATurret::Die);
	HealthComponent->onDamaged.AddUObject(this, &ATurret::DamageTaked);*/
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::Destroyed() 
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
};

void ATurret::Targeting() 
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if (CanFire() && Cannon && Cannon->IsReadyToFire())
	{
		Fire();
	}
};

void ATurret::RotateToPlayer() 
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
};

bool ATurret::IsPlayerInRange() 
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
};

bool ATurret::CanFire() 
{
	FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float DirAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir,DirToPlayer)));
	return DirAngle <= Accurency;
};
void ATurret::Fire() 
{
	if (Cannon)
	{
		Cannon->Fire();
	}
};

//void ATurret::Die()
//{
//	Destroy();
//}
//
//void ATurret::TakeDamage(FDamageData DamageData)
//{
//	UE_LOG(LogTankogeddon, Warning, TEXT("Turret %s take damage: %f"), *GetName(), DamageData.DamageValue);
//	HealthComponent->TankDamage(DamageData);
//}
//
//void ATurret::DamageTaked(float DamageValue)
//{
//	UE_LOG(LogTankogeddon, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
//}