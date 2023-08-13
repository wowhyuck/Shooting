// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTING_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AShooterCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void SetAiming(bool bIsAiming);
protected:
	virtual void BeginPlay() override;
	
	void FireButtonPressed(bool bPressed);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	class AShooterCharacter* Character;
	class AShooterPlayerController* Controller;
	class AShooterHUD* HUD;

	AWeapon* EquippedWeapon;

	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	FVector HitTarget;

	// HUD and Crosshairs
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	/* 조준 및 FOV */
	// 조준이 아닐 때 FOV(Field Of View), 시작할 때 카메라 기본 FOV 설정하기
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZooedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

public:	

		
};
