// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;
class USoundAttenuation;
class UAudioComponent;
class USoundBase;
class URocketMovementComponent;

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileRocket();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void BeginPlay() override;

	

private:

	

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	URocketMovementComponent* RocketMovementComponent;



	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* ProjectileLoop;

	UPROPERTY(VisibleAnywhere, Category = "Sounds")
	UAudioComponent* ProjectileLoopComponent;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundAttenuation* LoopingSoundAttenuation;

	




};
