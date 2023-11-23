// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"
#include "Shooting/Weapon/Weapon.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"


AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetOwner());
	AWeapon* EquippedWeapon = ShooterCharacter->GetEquippedWeapon();
	Damage = EquippedWeapon->GetDamage();
	HeadDamage = Damage * 1.5f;

	if (ShooterCharacter)
	{
		AController* ShooterController = ShooterCharacter->Controller;
		if (ShooterController)
		{
			// 적이 맞은 곳이 head일 때 true, 아닐 때 false
			const float DamageToCause = Hit.BoneName.ToString() == FString("head") ? HeadDamage : Damage;

			UGameplayStatics::ApplyDamage(OtherActor, DamageToCause, ShooterController, this, UDamageType::StaticClass());
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
