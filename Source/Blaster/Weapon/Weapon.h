// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

class USphereComponent;
class UWidgetComponent;
class UAnimationAsset;
class ACasing;
class UTexture2D;
class ABlasterPlayerController;
class ABlasterCharacter;

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget);
	void Dropped();

	virtual void Fire(const FVector& HitTarget);

	bool IsFull();

	/*
	Enable or Disable Custom depth
	*/

	void EnableCustomDepth(bool bEnable);

	bool bDestroyWeapon = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnWeaponStateSet();
	virtual void HandleOnEquip();
	virtual void HandleOnDropped();
	virtual void HandleOnEquippedSecondary();

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool isAutomatic = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = 0.15f;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "Components")
	UWidgetComponent* PickupWidget;

	UPROPERTY(VisibleAnywhere, Category = "States")
	EWeaponState WeaponState = EWeaponState::EWS_Initial;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* CrosshairsDown;

	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<ACasing> CasingClass;

	/*
	Ammo
	*/

	void SpendRound();

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MagCapacity;

	UPROPERTY(EditAnywhere, CAtegory = "Combat")
	EWeaponType WeaponType;

	/*
	* Players and Controllers
	*/

	UPROPERTY(VisibleAnywhere, Category = "Character")
	ABlasterCharacter* BlasterOwnerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	ABlasterPlayerController* BlasterOwnerController;

	UPROPERTY(EditAnywhere, CAtegory = "Combat")
	USoundBase* EquipSound;

public:	
	
	/*
	Getters
	*/
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const  { return SkeletalMesh; };
	FORCEINLINE UTexture2D* GetCrosshairsCenter() const  { return CrosshairsCenter; };
	FORCEINLINE UTexture2D* GetCrosshairsLeft() const { return CrosshairsLeft; };
	FORCEINLINE UTexture2D* GetCrosshairsRight() const { return CrosshairsRight; };
	FORCEINLINE UTexture2D* GetCrosshairsTop() const { return CrosshairsTop; };
	FORCEINLINE UTexture2D* GetCrosshairsDown() const { return CrosshairsDown; };
	FORCEINLINE float GetZoomFOV() const { return ZoomFOV; };
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; };
	FORCEINLINE float GetFireDelay() const { return FireDelay; };
	FORCEINLINE bool GetAutomatic() const { return isAutomatic; };
	FORCEINLINE int32 GetAmmo() const { return Ammo; };
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE EWeaponType GetWeaponType() const  { return WeaponType; };
	FORCEINLINE USoundBase* GetEquipSound() const { return EquipSound; }

	/*
	Setters
	*/
	void SetWeaponState(EWeaponState NewWeaponState);
	void SetHUDAmmo();
	FORCEINLINE void SetAmmo(int32 NewAmmo) { Ammo = NewAmmo; }
	
};
