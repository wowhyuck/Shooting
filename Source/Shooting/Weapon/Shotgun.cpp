// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "Shooting/Enemy/Enemy.h"
#include "Sound/SoundCue.h"


void AShotgun::Fire(const FVector& HitTarget)
{
	AWeapon::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket && InstigatorController)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		TMap<AEnemy*, uint32> HitMap;
		TMap<AEnemy*, uint32> HeadShotHitMap;

		for (uint32 i = 0; i < NumberOfPellets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			AEnemy* HitEnemy = Cast<AEnemy>(FireHit.GetActor());

			const bool bHeadShot = FireHit.BoneName.ToString() == FString("head");

			// 헤드샷일 때, 맞춘 적(Key)과 맞은 총알 개수(Value)를 HeadSHotHitMap에 저장하기
			if (bHeadShot)
			{
				if (HeadShotHitMap.Contains(HitEnemy)) HeadShotHitMap[HitEnemy]++;
				else HeadShotHitMap.Emplace(HitEnemy, 1);
			}
			// 헤드샷이 아닐 때(바디샷일 때), 맞춘 적(Key)과 맞은 총알 개수(Value)를 HitMap에 저장하기
			else
			{
				if (HitMap.Contains(HitEnemy)) HitMap[HitEnemy]++;
				else HitMap.Emplace(HitEnemy, 1);
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			}

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-0.5f, 0.5f));
			}
		}

		TArray<AEnemy*> HitEnemies;
		TMap<AEnemy*, float> DamageMap;

		// 바디샷일 때 저장된 HitMap의 맞춘 적(Key)과 적에게 적용된 데미지(Value, 맞은 총알 개수 * 총알 한 발당 데미지)를
		// DamageMap에 저장하기
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && InstigatorController)
			{
				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);

				HitEnemies.AddUnique(HitPair.Key);
			}
		}

		// 헤드샷일 때 저장된 HitMap의 맞춘 적(Key)과 적에게 적용된 데미지(Value, 맞은 총알 개수 * 총알 한 발당 헤드데미지)를
		// DamageMap의 저장된 데미지에 더해서 저장하기
		for (auto HeadShotHitPair : HeadShotHitMap)
		{
			if (HeadShotHitPair.Key)
			{
				if (DamageMap.Contains(HeadShotHitPair.Key)) DamageMap[HeadShotHitPair.Key] += HeadShotHitPair.Value * HeadDamage;
				else DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * HeadDamage);

				HitEnemies.AddUnique(HeadShotHitPair.Key);
			}
		}

		// 맞춘 적(Key)에게 데미지(Value)를 적용하기
		for (auto DamagePair : DamageMap)
		{
			if (DamagePair.Key && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					DamagePair.Key,
					DamagePair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass());
			}
		}
	}
}