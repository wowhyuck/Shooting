// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:	
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDWeaponName(FString WeaponName);
	void SetHUDWeaponIcon(UTexture2D* WeaponIcon);
	void SetHUDGold(int32 Gold);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDResultMenu();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	void OnMatchStateSet(FName State);
	void HandleMatchStarted();
	void HandleCooldown();

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

	void PollInt();

	void CheckMatchState();

private:	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AShooterHUD* ShooterHUD;

	UPROPERTY()
	class AShootingGameMode* ShootingGameMode;

	float LevelStartingTime = 0.f;
	float WarmupTime = 0.f;
	float MatchTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	FName MatchState;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	bool bInitializeCharacterOverlay = false;

	float HUDHealth;
	float HUDMaxHealth;
	bool bInitializeHealth = false;

	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;

	FString HUDWeaponName;
	class UTexture2D* HUDWeaponIcon;
	bool bInitializeWeapon = false;
};
