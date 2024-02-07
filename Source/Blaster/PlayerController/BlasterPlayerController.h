// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"


class ABlasterHUD;
class ABlasterCharacter;
class UCharacterOverlay;
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDGrenade(int32 Grenades);
	//void PollInit();

	void SetHUDCarriedWeaponAmmo(int32 Ammo);

	virtual void OnPossess(APawn* InPawn) override;

protected:

	virtual void BeginPlay() override;

	UCharacterOverlay* CharacterOverlay;

private:
	
	ABlasterHUD* BlasterHUD;

	//float HUDHealth;
	//float HUDMaxHealth;
	//float HUDScore;
	//int32 HUDDefeats;
	//int32 HUDGrenades;
};
