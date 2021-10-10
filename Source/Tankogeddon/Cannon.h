// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	int ProjectileAmmo = 5;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	uint8 TraceAmmo = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	EFireType FireType = EFireType::Casual;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	uint8 CountShotAutoType = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	TSubclassOf<AProjectile> ProjectileClass;
	

	FTimerHandle ReloadTimerHandle;
	bool bReadyToFire = false;
	FTimerHandle AutoFireTimer;
	uint8 CurrentCountShotAutoType = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire();
	bool IsReadyToFire();
	void FireSpecial();
	void CasualFire();
	void ChangeFireType();
	void AutoFire();
	void AutoTypeFire();

	protected:
	void Reload();

};
