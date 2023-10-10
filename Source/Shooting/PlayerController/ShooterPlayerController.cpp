// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Shooting/HUD/ShooterHUD.h"
#include "Shooting/HUD/CharacterOverlay.h"
#include "Shooting/HUD/Announcement.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "Shooting/GameMode/ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Shooting/ShooterComponents/CombatComponent.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CheckMatchState();
}

void AShooterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();

	PollInt();
}

void AShooterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = WarmupTime + MatchTime + CooldownTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		else if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

void AShooterPlayerController::PollInt()
{
	if (CharacterOverlay == nullptr)
	{
		if (ShooterHUD && ShooterHUD->CharacterOverlay)
		{
			CharacterOverlay = ShooterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);
				if (bInitializeWeapon)
				{
					SetHUDWeaponName(HUDWeaponName);
					SetHUDWeaponIcon(HUDWeaponIcon);
				}
			}
		}
	}
}

void AShooterPlayerController::CheckMatchState()
{
	AShootingGameMode* GameMode = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();

		ShooterHUD = Cast<AShooterHUD>(GetHUD());
		if (ShooterHUD && MatchState == MatchState::WaitingToStart)
		{
			ShooterHUD->AddAnnouncement();
		}
	}
}

void AShooterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD && 
		ShooterHUD->CharacterOverlay && 
		ShooterHUD->CharacterOverlay->HealthBar && 
		ShooterHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		ShooterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		ShooterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AShooterPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD &&
		ShooterHUD->CharacterOverlay &&
		ShooterHUD->CharacterOverlay->WeaponAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoAmountText = FString::Printf(TEXT("%d"), Ammo);
		ShooterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoAmountText));
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void AShooterPlayerController::SetHUDWeaponName(FString WeaponName)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD &&
		ShooterHUD->CharacterOverlay &&
		ShooterHUD->CharacterOverlay->WeaponName;

	if (bHUDValid)
	{
		ShooterHUD->CharacterOverlay->WeaponName->SetText(FText::FromString(WeaponName));
	}
	else
	{
		bInitializeWeapon = true;
		HUDWeaponName = WeaponName;
	}
}

void AShooterPlayerController::SetHUDWeaponIcon(UTexture2D* WeaponIcon)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD &&
		ShooterHUD->CharacterOverlay &&
		ShooterHUD->CharacterOverlay->WeaponIcon;
	if (bHUDValid)
	{
		ShooterHUD->CharacterOverlay->WeaponIcon->SetBrushFromTexture(WeaponIcon);
	}
	else
	{
		bInitializeWeapon = true;
		HUDWeaponIcon = WeaponIcon;
	}
}

void AShooterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD &&
		ShooterHUD->CharacterOverlay &&
		ShooterHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			ShooterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		ShooterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void AShooterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

	bool bHUDValid = ShooterHUD &&
		ShooterHUD->Announcement &&
		ShooterHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			ShooterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Seconds = CountdownTime;

		FString CountdownText = FString::Printf(TEXT("%d"), Seconds);
		ShooterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(InPawn);
	if (ShooterCharacter)
	{
		SetHUDHealth(ShooterCharacter->GetHealth(), ShooterCharacter->GetMaxHealth());
	}
}

void AShooterPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AShooterPlayerController::HandleMatchStarted()
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD)
	{
		ShooterHUD->AddCharacterOverlay();
		if (ShooterHUD->Announcement)
		{
			ShooterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AShooterPlayerController::HandleCooldown()
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD)
	{
		ShooterHUD->CharacterOverlay->RemoveFromParent();

		bool bHUDValid = ShooterHUD->Announcement &&
			ShooterHUD->Announcement->AnnouncementText1 &&
			ShooterHUD->Announcement->AnnouncementText2;
		if (bHUDValid)
		{
			ShooterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText(TEXT("게임 재시작"));
			ShooterHUD->Announcement->AnnouncementText1->SetText(FText::FromString(AnnouncementText));
			ShooterHUD->Announcement->AnnouncementText2->SetText(FText());
			ShooterHUD->Announcement->WarmupTime->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ShooterCharacter && ShooterCharacter->GetCombat())
	{
		ShooterCharacter->bDisableGameplay = true;
		ShooterCharacter->GetCombat()->FireButtonPressed(false);
	}
}
