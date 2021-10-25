// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStructs.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "MilitaryEquipment.h"
#include "Engine/TargetPoint.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public AMilitaryEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params| Patrol points", meta = (MakeEditWidget = true))
	TArray<class ATargetPoint*> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params| Accurency")
	float MovementAccurency = 50;
	

	UPROPERTY()
	bool SelectedCannon = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	class ACannon* FirstCannon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	class ACannon* SecondCannon = nullptr;

	UFUNCTION(BlueprintPure, Category = "Turret")
	class ACannon* GetFirstCannon() const;

	UFUNCTION()
	TArray<FVector> GetPatrollingPoints()
	{
		TArray<FVector> Points;
		for (ATargetPoint* Point : PatrollingPoints)
		{
			Points.Add(Point->GetActorLocation());
		}

		return Points;
	}

	UFUNCTION()
	void SetPatrollingPoints(TArray<class ATargetPoint*> NewPatrollingPoints);


	UFUNCTION()
	float GetMovementAccurency()
	{
		return MovementAccurency;
	}

	UFUNCTION()
	void ChangeWeapon();

	UFUNCTION()
	void AddAmmo();

	UFUNCTION()
	FVector GetTurretForwardVector();

	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition();



private:

	float CurrentMoveForwardAxis = 0.f;

	float TargetMoveForwardAxis = 0.f;

	float CurrentRotateRightAxis = 0.f;

	float TargetRotateRightAxis = 0.f;

	FVector TurretTargetPosition;
};