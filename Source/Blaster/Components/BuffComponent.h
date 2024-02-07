// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	friend class ABlasterCharacter;
	void Heal(float HealAmount, float HealingTime);
	void ShieldBuff(float ShieldAmount, float ShieldTime);
	void Speed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);
	void JumpBuff(float BuffBaseJump, float BuffJumpTime);
	

protected:

	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);

	void ShieldRampUp(float DeltaTime);

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void ResetSpeeds();
	void ResetJump();

	float InitialJump;
	FTimerHandle JumpBuffTimer;

	float InitialBaseSpeed;
	float InitialCrouchSpeed;
	FTimerHandle SpeedBufftimer;

	bool bHealing = false;
	float HealingRate = 0;
	float AmountToHeal = 0.f;

	bool bShielding = false;
	float ShieldRate = 0;
	float AmountToShield = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Characters")
	ABlasterCharacter* Character;

public:

	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);
	void SetInitialJumpVelocity(float Velocity);
	
};
