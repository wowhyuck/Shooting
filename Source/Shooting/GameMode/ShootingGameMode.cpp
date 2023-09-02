// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Shooting/PlayerController/ShooterPlayerController.h"


AShootingGameMode::AShootingGameMode()
{
	bDelayedStart = true;
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AShootingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void AShootingGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	AShooterPlayerController* ShooterPlayer = Cast<AShooterPlayerController>(*(GetWorld()->GetPlayerControllerIterator()));
	if (ShooterPlayer)
	{
		ShooterPlayer->OnMatchStateSet(MatchState);
	}
}
