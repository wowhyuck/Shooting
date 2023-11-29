// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffComponent::Heal(float HealAmount)
{
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealAmount, 0.f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();
}

//void UBuffComponent::TakeGold(int32 GoldAmount)
//{
//	Character->SetGold(FMath::Clamp(Character->GetGold() + GoldAmount, 0, Character->GetMaxGold()));
//}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}