// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

class UParticleSystem;
/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Particles")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "Particles")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.f;

private:

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* BeamParticles;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UParticleSystemComponent* BeamParticlesComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* MuzzleFlash;

	

	/*
	Trace end with scatter
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bUseScatter = false;
};
