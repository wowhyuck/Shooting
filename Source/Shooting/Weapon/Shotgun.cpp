// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "Shooting/Enemy/Enemy.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"


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
			ShotgunTraceHit(Start, HitTarget, FireHit);

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
				DamageMap.Emplace(HitPair.Key, HitPair.Value * GetDamage());

				HitEnemies.AddUnique(HitPair.Key);
			}
		}

		// 헤드샷일 때 저장된 HitMap의 맞춘 적(Key)과 적에게 적용된 데미지(Value, 맞은 총알 개수 * 총알 한 발당 헤드데미지)를
		// DamageMap의 저장된 데미지에 더해서 저장하기
		for (auto HeadShotHitPair : HeadShotHitMap)
		{
			if (HeadShotHitPair.Key)
			{
				HeadDamage = GetDamage() * 1.5f;
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

FVector AShotgun::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();		// 총알 방향
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;		// 총 퍼짐 범위(구)
	FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);		// 구를 중심으로 무작위 방향
	FVector EndLoc = SphereCenter + RandVec;		// 구 표면의 무작위 점(무작위 방향에 따른)
	FVector ToEndLoc = EndLoc - TraceStart;		// 총구부터 무작위 점을 향한 벡터

	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}

void AShotgun::ShotgunTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector End = TraceEndWithScatter(TraceStart, HitTarget);

		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility);

		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator,
				true);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}
