// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CharacterTypes.h"
#include "CombatComponent.generated.h"



class ABlasterCharacter;
class ABlasterPlayerController;
class ABlasterHUD;
class AWeapon;
class USoundBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ABlasterCharacter;
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
	bool ShouldSwapWeapons();

protected:

	virtual void BeginPlay() override;
	void EquipWeapon(AWeapon* WeaponToEquip);
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);
	void UpdateCarryAmmo();
	void UpdateThrowGrenades();
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachActorToBackpack(AActor* ActorToAttach);
	void DropEquippedWeapon();
	void ReloadWhenEmpty();
	void SetAiming(bool bAiming);
	void FireButtonPressed(bool bFirePressed);
	void Fire();
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);
	void InterpFOV(float DeltaTime);
	void ThrowGrenade();
	void ShowAttachedGrenade(bool bShowGrenade);
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);
	

	UFUNCTION(BlueprintCallable)
	void GrenadeLaunch();

	void UpdateShotgunAmmoValues();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AProjectile> GrenadeClass;

private:

	bool isFireButtonPressed;
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	float BaseSpread = 0.5f;
	FVector HitTarget;
	FHUDPackage HUDPackage;
	float DefaultFOV;
	float CurrentFOV;
	FTimerHandle FireTimer;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bcanFire = true;

	bool canFire();

	void StartFireTimer();
	void FinishedFireTimer();
	

	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category = "Characters")
	ABlasterCharacter* Character;

	UPROPERTY(VisibleAnywhere, Category = "Controllers")
	ABlasterPlayerController* BlasterPlayerController;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	ABlasterHUD* BlasterHUD;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* SecondaryWeapon;

	UPROPERTY(VisibleAnywhere,Category = "Combat")
	bool isAiming;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AimWalkSpeed = 350.f;

	/*
	Ammo
	*/


	void InitializeCarriedAmmo();
	int32 AmountToReload();
	void AddAmmo(int32 AmmoToAdd);

	TMap<EWeaponType, int32> WeaponAmmoMap;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingARAmmo = 180;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingRocketAmmo = 8;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingPistolAmmo = 10;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingSubmachineAmmo = 30;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingShotgunAmmo = 10;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingSniperRifleAmmo = 5;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingGranadeLauncherAmmo = 5;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 CarriedAmmo;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 CarriedGrenades = 1;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxGrenades = 4;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxCarriedAmmo = 500;




public:	
	
	FORCEINLINE int32 GetGrenades() const { return CarriedGrenades; }
	

		
};
