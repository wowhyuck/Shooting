// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTING_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventIntigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bStunned = false;

	// 플레이어가 공격 범위 안에 들어올 때 true, 공격 시작 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* CombatRangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* LeftWeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* RightWeaponCollision;

	void Die();

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void PlayHitMontage(FName Section, float PlayRate = 1.0f);

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage(FName Section, float PlayRate = 1.0f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

	void MoveToShooter();

	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);

	UFUNCTION()
	void CombatRangeOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CombatRangeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintPure)
	FName GetAttackSectionName();

	UFUNCTION()
	void OnLeftWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere)
	FString HeadBone;

	UPROPERTY(EditAnywhere)
	float HealthBarDisplayTime = 4.0f;

	FTimerHandle HealthBarTimer;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontage; 

	FTimerHandle HitReactTimer;
	bool bCanHitReact = true;

	UPROPERTY(EditAnywhere)
	float HitReactTimerMin = 0.5f;

	UPROPERTY(EditAnywhere)
	float HitReactTimerMax = 1.0f;

	UPROPERTY(VisibleAnywhere)
	TArray<UUserWidget*> HitNumbers;

	UPROPERTY(EditAnywhere)
	float HitNumberDestroyTime = 0.5f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	class AEnemyController* EnemyController;

	// StunChance = 0 : 기절 확률 0%, = 1 : 기절 확률 100% 
	UPROPERTY(EditAnywhere)
	float StunChance = 0.5f;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	// Attack Montage의 섹션 이름
	FName AttackLFast = TEXT("AttackLFast");
	FName AttackRFast = TEXT("AttackRFast");
	FName AttackL = TEXT("AttackL");
	FName AttackR = TEXT("AttackR");

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation);

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
