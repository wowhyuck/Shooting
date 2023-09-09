#pragma once

#define TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssultRifle UMETA(DisplayName = "Assult Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};