﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponName;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Gold;

};
