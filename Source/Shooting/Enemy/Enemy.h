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

	void Die();

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void PlayHitMontage(FName Section, float PlayRate = 1.0f);

	void ResetHitReactTimer();

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

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation);
};
