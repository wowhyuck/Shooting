﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "Engine/DataTable.h"
#include "Weapon.generated.h"


/* 무기 장착 상태 (주 무기, 보조 무기)*/
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),

	EWS_MAX UMETA(DisplayName = "DefaultMax")
};


USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagCapacityPerLevel;
};

UCLASS()
class SHOOTING_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;

	virtual void Fire(const FVector& HitTarget);		// 사격 함수
	void SetHUDAmmo();		// 탄약 HUD 세팅 함수
	void AddAmmo(int32 AmmoToAdd);		// 탄약 추가 함수
	void SetHUDWeapon();		// 무기 이름, 이미지 HUD 세팅 함수

	/* 무기 조준점의 텍스처 변수 */
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	/* 조준중 줌인 FOV */
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	/* 자동 연사 */
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = 0.15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

	// 무기 장착 소리
	UPROPERTY(EditAnywhere)
	USoundCue* EquipSound;

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/* 무기 관련 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* WeaponIcon;

	/* 탄약 관련 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MagCapacity;

	/* 무기 장착 상태에 따른 함수 */
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnEquippedSecondary();

	// 무기 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* WeaponDataTable;

private:
	UPROPERTY()
	class AShooterCharacter* ShooterOwnerCharacter;

	UPROPERTY()
	class AShooterPlayerController* ShooterOwnerController;

	/* 무기 구성 관련 변수 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	// 연사 애니메이션
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	/* 레벨 관련 변수 */
	UPROPERTY(EditAnywhere)
	int32 WeaponLevel = 1;

	float DamagePerLevel;

	int32 MagCapacityPerLevel;

	// 사격했을 때 탄약 감소
	void SpendRound();

public:	
	FORCEINLINE void SetWeaponState(EWeaponState State);
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	bool IsEmpty();
	bool IsFull();
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE void SetAmmo(int32 AmmoAmount) { Ammo = AmmoAmount; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE bool IsEmpty() const { return Ammo == 0; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE void SetDamage(float DamageAmount) { Damage = DamageAmount; }
};
