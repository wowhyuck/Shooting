// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooting/ShootingTypes/TurningInPlace.h"
#include "Shooting/ShootingTypes/CombatState.h"
#include "Components/TimelineComponent.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTING_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	/* Montage 실행 함수 */
	void PlayFireMontage(bool bAiming);
	void PlayHitReactMontage();
	void PlayReloadMontage();
	void PlaySwapMontage();
	void PlayDieMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	/* HUD 업데이트 함수 */
	void UpdateHUDHealth();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* MeleeImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AWeapon* StartingWeapon;

	/* 캐릭터 이동 & 회전 함수 */
	virtual void Jump() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void AimOffset(float DeltaTime);
	void TurnInPlace(float DeltaTime);

	/* 입력키 함수 */
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	void SwapButtonPressed();

	// 피해 데미지 함수
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	
	/* 죽기 함수 */
	void Die();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	/* HUD 업데이트 함수 */
	void UpdateHUDAmmo();

	// 기본 무기 장착 함수
	void SpawnDefaultWeapon();

private:
	class AShooterPlayerController* ShooterPlayerController;
	class AShootingGameMode* ShootingGameMode;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBuffComponent* Buff;

	// AimOffset 관련 변수
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	ETurningInPlace TurningInPlace;

	/* 애니메이션 몽타주 */
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* DieMontage;

	/* 플레이어 체력 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float Health = 100.f;

	/* 플레이어 골드 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 MaxGold = 1000;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 Gold = 0;

	/* Weapons */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> DefaultWeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> SecondaryWeaponClass;

	// 죽기 여부
	bool bDead = false;

	/* Dissolve 효과 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;		// 런타임 때 바꿀 수 있는 Dynamic Instance

	UPROPERTY(EditAnywhere)
	UMaterialInstance* DissolveMaterialInstance;		// Dynamic material instance와 함께 사용할 블루프린트에서 세팅된 Material Instance

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

public:	
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float HealthAmount) { Health = HealthAmount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetGold() const { return Gold; }
	FORCEINLINE void SetGold(float GoldAmount) { Gold = GoldAmount; }
	FORCEINLINE float GetMaxGold() const { return MaxGold; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE bool IsDead() const { return bDead; }
	FORCEINLINE USoundCue* GetMeleeImpactSound() const { return MeleeImpactSound; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
}; 
