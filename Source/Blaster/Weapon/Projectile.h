// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	void SpawnBulletParticles();
	void SpawnTrailSystem();
	void StartDestroyTimer();

	virtual void DestroyTimerFinished();
	void ExplodeDamage();
	

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ExplosionInnerRadiusDamage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ExplosionOuterRadiusDamage = 150.f;

	UPROPERTY(EditAnywhere, Category = "Particles")
	float DestroyTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UNiagaraSystem* TrailSystem;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 10;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnImpactSoundParticles();

	

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* Tracer;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ImpactSound;

private:

	


	
};
