// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	// 총 퍼짐 함수
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);
	void ShotgunTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 NumberOfPellets = 10;

	/* 총 퍼짐 */
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;
};
