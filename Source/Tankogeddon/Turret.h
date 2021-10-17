// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryEquipment.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public AMilitaryEquipment
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY()
	class ACannon* Cannon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	float TargetingRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	float Accurency = 10.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();

};
