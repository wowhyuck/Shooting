// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAnimInstance.h"
#include "ShootingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UShootingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShootingCharacter = Cast<AShootingCharacter>(TryGetPawnOwner());
}

void UShootingAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ShootingCharacter == nullptr)
	{
		ShootingCharacter = Cast<AShootingCharacter>(TryGetPawnOwner());
	}
	if (ShootingCharacter == nullptr) return;

	// 캐릭터 이동, 점프 관련 변수 초기화
	FVector Velocity = ShootingCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ShootingCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ShootingCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}