// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooting/Character/ShooterCharacter.h"
#include "Shooting/Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WeaponType.h"
#include "DrawDebugHelpers.h"


void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket && InstigatorController)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		
		TArray<FHitResult> FireHits;
		AEnemy* HitEnemy;
		HeadDamage = GetDamage() * 2.0f;

		if (GetWeaponType() == EWeaponType::EWT_SniperRifle)
		{
			SniperRifleTraceHit(Start, HitTarget, FireHits);
			
			for (const auto FireHit : FireHits)
			{
				HitEnemy = Cast<AEnemy>(FireHit.GetActor());
				if (HitEnemy)
				{
					// 적이 맞은 곳이 head일 때 true, 아닐 때 false
					const float DamageToCause = FireHit.BoneName.ToString() == FString("head") ? HeadDamage : GetDamage();
					UGameplayStatics::ApplyDamage(
						HitEnemy,
						DamageToCause,
						InstigatorController,
						this,
						UDamageType::StaticClass());
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
						FireHit.ImpactPoint);
				}
			}
		}

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				SocketTransform);
		}

		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation());
		}
	}
}

void AHitScanWeapon::SniperRifleTraceHit(const FVector& TraceStart, const FVector& HitTarget, TArray<FHitResult>& OutHits)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

		World->LineTraceMultiByChannel(
			OutHits,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility);

		FVector BeamEnd = End;
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