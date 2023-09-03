// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShootingGameMode.generated.h"


namespace MatchState
{
	extern SHOOTING_API const FName Cooldown;	// Match가 끝나고 결과 HUD를 띄움
}

/**
 * 
 */
UCLASS()
class SHOOTING_API AShootingGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AShootingGameMode();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	float CountdownTime = 0.f;
};
