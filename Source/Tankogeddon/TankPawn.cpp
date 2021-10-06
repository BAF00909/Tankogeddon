// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankogeddonLog.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannoon setup"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Move tank forward/back 
	FVector MovePersonPosition= GetActorLocation()+ GetActorForwardVector() * MoveSpeed * TargetForwardAxisValue * DeltaTime;
	SetActorLocation(MovePersonPosition, true);

	//Rotate tank
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRotateRightAxisValue, InterpolationKey);
	float YawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator CurrentRotation = GetActorRotation();
	YawRotation = CurrentRotation.Yaw + YawRotation;
	FRotator NewRotation = FRotator(0.f, YawRotation, 0.f);
	
	SetActorRotation(NewRotation);

	//Rotate turret of tank
	if(TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
		FRotator CurrentRotationTurret = TurretMesh->GetComponentRotation();
		TargetRotation.Pitch = CurrentRotationTurret.Pitch;
		TargetRotation.Roll = CurrentRotationTurret.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotationTurret, TargetRotation, TurretRotationInterPolationKey));
	}
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	TargetRotateRightAxisValue = AxisValue;
}

void ATankPawn::SetupCannon()
{
	if(Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::ChangeFireType()
{
	if(Cannon)
	{
		Cannon->ChangeFireType();
	}
}




