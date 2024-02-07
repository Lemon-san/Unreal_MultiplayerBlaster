#pragma once

#define TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "IntialState"),
	EWS_Equip UMETA(DisplayName = "Equipped"),
	EWS_EquipSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	EWT_RocketLauncher UMETA(DisplayName = "RocketLauncher"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SubmachineGun UMETA(DisplayName = "SubmachineGun"),
	EWT_SniperRifle UMETA(DisplayName = "SniperRifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_GranadeLauncher UMETA(DisplayName = "GranadeLauncher"),
	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};