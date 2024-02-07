// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;
class USoundBase;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class BLASTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APickup();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:

	virtual void BeginPlay() override;

	void StartOverlapTimer();

	void BindOverlapTimerFinished();

	FTimerHandle BindOverlapTimer;

	float BindOverlapTime = 0.25f;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere, Category = "Components")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, Category = "Component")
	float BaseTurnRate = 45.f;

	UPROPERTY(EditAnywhere, Category = "Particles/Effects")
	UNiagaraComponent* PickupEffectComponent;

	UPROPERTY(EditAnywhere, Category = "Particles/Effects")
	UNiagaraSystem* PickupEffectSystem;



public:	
	

};
