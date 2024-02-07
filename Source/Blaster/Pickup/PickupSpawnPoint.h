// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class BLASTER_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	

	APickupSpawnPoint();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	void SpawnPickup();
	
	void SpawnPickupTimerFinished();

	UFUNCTION()
	void SpawnPickupTimerStart(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere, Category = "Pickups")
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
	APickup* SpawnedPickup;

private:

	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditAnywhere, Category = "Pickups")
	float SpawnPickupTimeMin;

	UPROPERTY(EditAnywhere, Category = "Pickups")
	float SpawnPickupTimeMax;


public:	


};
