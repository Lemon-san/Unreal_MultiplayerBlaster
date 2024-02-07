// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Blaster/Interfaces/CrosshairsInterface.h"
#include "Blaster/BlasterTypes/CharacterTypes.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"


class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class AWeapon;
class UCombatComponent;
class UBuffComponent;
class UBlasterAnimInstance;
class UAnimMontage;
class ABlasterPlayerController;
class ABlasterGameMode;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public ICrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void UpdateHUDHealth();
	void UpdateHUDShield();
	bool isWeaponEquipped();
	//bool isAiming();
	bool isCharacterAiming();
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	void PlayHitReactMontage();
	FVector GetHitTarget() const;
	void Reload();
	void Elim();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void PlayTossGrenade();
	void SpawnDefaultWeapon();


	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

protected:

	virtual void BeginPlay() override;
	void HideGrenade();
	
	void UpdateHUDAmmo();

	//Poll for any relevant classes and initialize HUD
	void PollInit();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	virtual void Jump() override;
	void SpawnEquip();
	void CrouchPressed();
	void Aim();
	void AimPressed();
	void SwitchWeaponPressed();
	void AimReleased();
	void AimOffset(float DeltaTime);
	void TurnInPlace(float DeltaTime);
	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();

	void PlayReloadMontage();




	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);

	float HipAimOffsetYaw;
	float HipAimOffsetPitch;
	float InterpOffsetYaw;

	

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> DefaultWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (ClampMin = "0.0", ClampMax = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BaseLookRate = 0.9f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBuffComponent* BuffComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* AttachGrenade;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* TossGrenadeAction;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* TossGrenadeMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Aim")
	FRotator StartingAimRotation;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* SpawnWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (ClampMin = "0.05", ClampMax = "2.0", UIMin = "0.05", UIMax = "2.0"))
	float HipLookRate = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Types")
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

private:

	void ElimTimerFinished();

	void HideCamera();

	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	ABlasterPlayerController* BlasterPlayerController;

	bool isElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Elimination")
	float ElimDelay = 3.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraThreshold = 200.f;


	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	float Shield = 100.f;

	/*
	* Dissolve Effect
	*/

	//can change at runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	//set on blueprint used with dynamic
	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* DissolveMaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UTimelineComponent* DissolveTimeline;

	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	/*
	* ElimBot
	*/
	UPROPERTY(EditAnywhere, Category = "Elim")
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere, Category = "Elim")
	USoundBase* ElimBotSound;

	/*
	PlayerState
	*/
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	class ABlasterPlayerState* BlasterPlayerState;


public:

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; };
	FORCEINLINE float GetHipAimOffsetYaw() const { return HipAimOffsetYaw; };
	FORCEINLINE float GetHipAimOffsetPitch() const { return HipAimOffsetPitch; };
	FORCEINLINE ETurningInPlace GetTurningInplace() const { return TurningInPlace; };
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; };
	FORCEINLINE bool GetisElimmed() const { return isElimmed; };
	FORCEINLINE float GetHealth() const { return Health; };
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };
	FORCEINLINE float GetShield() const { return Shield; };
	FORCEINLINE float GetMaxShield() const { return MaxShield; };
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; };
	FORCEINLINE UStaticMeshComponent* GetGrenadeMesh() const { return AttachGrenade; };
	FORCEINLINE ABlasterPlayerState* GetBlasterPlayerState() const { return BlasterPlayerState; };
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; };
	FORCEINLINE UBuffComponent* GetBuffComponent() const { return BuffComponent; };
	ECombatState GetCombatState() const;
	AWeapon* GetEquippedWeapon();

	//Setters
	FORCEINLINE void SetHealth(float HealthAmount) { Health = HealthAmount; }
	FORCEINLINE void SetShield(float ShieldAmount) { Shield = ShieldAmount; }
};