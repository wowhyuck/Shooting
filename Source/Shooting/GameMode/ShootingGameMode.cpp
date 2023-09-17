// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Shooting/PlayerController/ShooterPlayerController.h"


// MatchState Custom
namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AShootingGameMode::AShootingGameMode()
{
	bDelayedStart = true;		// 게임이 바로 시작 방지
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AShootingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)		// 게임 시작 전 준비
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)		// 게임 시작
	{
		CountdownTime = WarmupTime + MatchTime- GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)		// 게임 종료 및 결과
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		
		// TODO : 재시작 버튼을 클릭했을 때 게임 재시작하기
		if (CountdownTime <= 0.f)
		{
			RestartGame();
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
