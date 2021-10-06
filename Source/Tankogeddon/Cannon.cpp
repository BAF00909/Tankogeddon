// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = SceeneCpm;
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	CannonMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon spawn point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

}

void ACannon::Fire()
{
	if(FireType == EFireType::Casual)
	{
		CasualFire();
	}
	else
	{
		AutoFire();
	}
}

void ACannon::FireSpecial()
{
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Fire special");
}

void ACannon::CasualFire()
{
	if(!bReadyToFire)
	{
		return;
	}
	bReadyToFire = false;
	if(CannonType == ECannonType::FireProjectile)
	{
		if(ProjectileAmmo == 0)
		{
			GEngine->AddOnScreenDebugMessage(10,1,FColor::Red, "No shells");
			return;
		}
		GEngine->AddOnScreenDebugMessage(10,1,FColor::Green, "Fire prijectile");
		--ProjectileAmmo;
		UE_LOG(LogTemp, Warning, TEXT("Ammo %d"), ProjectileAmmo);
	}
	else
	{
		if(TraceAmmo == 0)
		{
			GEngine->AddOnScreenDebugMessage(10,1,FColor::Red, "No shells");
			return;
		}
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire trace");
		--TraceAmmo;
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1/FireRate, false);
}

void ACannon::ChangeFireType()
{
	if(FireType == EFireType::Casual)
	{
		FireType = EFireType::Auto;
	}
	else
	{
		FireType = EFireType::Casual;
	}
}

void ACannon::AutoFire()
{
	GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &ACannon::AutoTypeFire, 1.0, true);
}

void ACannon::AutoTypeFire()
{
	
	if(CurrentCountShotAutoType == CountShotAutoType)
	{
		UE_LOG(LogTemp, Warning, TEXT("STOP FIRE"));
		GetWorldTimerManager().ClearTimer(AutoFireTimer);
		CurrentCountShotAutoType = 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Boom: %d"), CurrentCountShotAutoType);
		GEngine->AddOnScreenDebugMessage(10, 0.5, FColor::Blue, "Fire");
		++CurrentCountShotAutoType;
	}
}


bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

