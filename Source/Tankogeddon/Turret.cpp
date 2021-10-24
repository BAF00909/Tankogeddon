// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "MilitaryEquipment.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATurret::ATurret(){}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Tick(float DeltaTime)
{
	Targeting();
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
	
	if (!IsPlayerInRange())
	{
		RotateToPlayer();
	}
	if (CanFire() && Cannon && Cannon->IsReadyToFire() && IsPlayerInRange())
	{
		Fire();
	}
};

void ATurret::RotateToPlayer() 
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	CurrentRotation.Pitch = TargetRotation.Pitch;
	CurrentRotation.Roll = TargetRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
};

bool ATurret::IsPlayerInRange() 
{
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return false;
	}
	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Turret Vission Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Cyan, false, 0.1f, 0, 5);
		if (HitResult.Actor == PlayerPawn)
		{
			return true;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 0.1f, 0, 5);
	}

	return false;
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

FVector ATurret::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

