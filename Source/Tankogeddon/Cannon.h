// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
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
	int TraceAmmo = 5;


	FTimerHandle ReloadTimerHandle;
	bool bReadyToFire = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire();
	bool IsReadyToFire();
	void FireSpecial();

	protected:
	void Reload();

};
