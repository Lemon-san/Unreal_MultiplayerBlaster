// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BuffPickup.generated.h"


/**
 * 
 */
UCLASS()
class BLASTER_API ABuffPickup : public APickup
{
	GENERATED_BODY()
	
public:
	ABuffPickup();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HealAmount = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HealingTime = 5.f;
};
