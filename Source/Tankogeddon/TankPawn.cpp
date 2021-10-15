// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include <GameFramework/SpringArmComponent.h>
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include <Camera/CameraComponent.h>
#include "Math/UnrealMathUtility.h"
#include "Tankogeddon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"

// Sets default values
ATankPawn::ATankPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->onDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->onDamaged.AddUObject(this, &ATankPawn::DamageTaked);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(DefaultCannonClass);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime, MovementSmootheness);
	FVector MoveVector = GetActorForwardVector() * CurrentMoveForwardAxis;
	FVector NewActorLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
	SetActorLocation(NewActorLocation, true);

	CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime, RotationSmootheness);
	float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	UE_LOG(LogTankogeddon, Verbose, TEXT("CurrentRotateRightAxis: %f"), CurrentRotateRightAxis);

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurretRotationSmootheness));
}

void ATankPawn::MoveForward(float InAxisValue)
{
	TargetMoveForwardAxis = InAxisValue;
}

void ATankPawn::RotateRight(float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::Fire()
{
	if (FirstCannon)
	{
		FirstCannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (FirstCannon)
	{
		FirstCannon->FireSpecial();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass)
{
	if (FirstCannon)
	{
		FirstCannon->Destroy();
	}
	if (InCannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		FirstCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
		FirstCannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ATankPawn::ChangeWeapon()
{
	Swap(FirstCannon, SecondCannon);
	if (FirstCannon)
	{
		FirstCannon->SetVisibility(true);
	}

	if (SecondCannon)
	{
		SecondCannon->SetVisibility(false);
	}
}

ACannon* ATankPawn::GetFirstCannon() const
{
	return FirstCannon;
}

void ATankPawn::AddAmmo()
{
	
	if (FirstCannon)
	{
		FirstCannon->AddAmmo();
	}

}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TankDamage(DamageData);
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}