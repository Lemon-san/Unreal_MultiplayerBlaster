// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/BlasterTypes/DebugMacros.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Blaster/Character/BlasterAnimInstance.h"
#include "Blaster/Weapon/Projectile.h"
#include "Sound/SoundBase.h"



UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character && Character->GetCamera())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		DefaultFOV = Character->GetCamera()->FieldOfView;
		CurrentFOV = DefaultFOV;
	}
	UpdateThrowGrenades();

	InitializeCarriedAmmo();
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
	
}

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	if (WeaponAmmoMap.Contains(WeaponType))
	{
		WeaponAmmoMap[WeaponType] = FMath::Clamp(WeaponAmmoMap[WeaponType] + AmmoAmount, 0, MaxCarriedAmmo);
		UpdateCarryAmmo();
	}

	if (EquippedWeapon && EquippedWeapon->GetAmmo() <= 0 && EquippedWeapon->GetWeaponType() == WeaponType)
	{
		Character->Reload();
	}
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return EquippedWeapon && SecondaryWeapon;
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (!Character || !Character->Controller) return;

	BlasterPlayerController = (BlasterPlayerController == nullptr) ? (Cast<ABlasterPlayerController>(Character->Controller)) : (BlasterPlayerController);

	if (BlasterPlayerController)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : BlasterHUD;

		if (BlasterHUD)
		{
			

			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->GetCrosshairsCenter();
				HUDPackage.CrosshairsLeft = EquippedWeapon->GetCrosshairsLeft();
				HUDPackage.CrosshairsRight = EquippedWeapon->GetCrosshairsRight();
				HUDPackage.CrosshairsTop = EquippedWeapon->GetCrosshairsTop();
				HUDPackage.CrosshairsDown = EquippedWeapon->GetCrosshairsDown();
			}

			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsDown = nullptr;
			}

			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
			
			if (Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}
			if (isAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			HUDPackage.CrosshairSpread = BaseSpread + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;

			BlasterHUD->SetHUDPackage(HUDPackage);
			
		}
	}

}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (!EquippedWeapon) return;

	if (isAiming)
	{
		//interps zooms at unique weapon speeds
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());

	}
	else
	{
		//interps Unzooms for all weapons at the same speed
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	if (Character && Character->GetCamera())
	{
		Character->GetCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::ThrowGrenade()
{
	if (CombatState != ECombatState::ECS_Unoccupied || !EquippedWeapon || !(CarriedGrenades > 0)) return;
	
	CombatState = ECombatState::ECS_TossGrenade;
	if (Character)
	{
		ShowAttachedGrenade(true);
		Character->PlayTossGrenade();
		AttachActorToLeftHand(EquippedWeapon);
		CarriedGrenades = FMath::Clamp(CarriedGrenades - 1, 0, MaxGrenades);
		UpdateThrowGrenades();
	}
	

}

void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
{
	if (Character && Character->GetGrenadeMesh())
	{
		Character->GetGrenadeMesh()->SetVisibility(bShowGrenade);
	}
}



void UCombatComponent::GrenadeLaunch()
{
	ShowAttachedGrenade(false);
	if (Character && GrenadeClass && Character->GetGrenadeMesh())
	{
		const FVector StartingLocation = Character->GetGrenadeMesh()->GetComponentLocation();
		FVector ToTarget = HitTarget - StartingLocation;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Character;
		SpawnParams.Instigator = Character;

		UWorld* World = GetWorld();

		if (World)
		{
			World->SpawnActor<AProjectile>(GrenadeClass,StartingLocation,ToTarget.Rotation(),SpawnParams);
		}

	}
}

bool UCombatComponent::canFire()
{
	if (EquippedWeapon == nullptr ) return false;

	if (EquippedWeapon->GetAmmo() > 0 && bcanFire && CombatState == ECombatState::ECS_Reloading && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun)
	{
		CombatState = ECombatState::ECS_Unoccupied;
		return true;
	}

	return CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon->GetAmmo() > 0 && bcanFire;
}

void UCombatComponent::StartFireTimer()
{
	if (!EquippedWeapon || !Character) return;

	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FinishedFireTimer, EquippedWeapon->GetFireDelay());
	
}

void UCombatComponent::FinishedFireTimer()
{
	bcanFire = true;

	if (isFireButtonPressed && EquippedWeapon->GetAutomatic())
	{
		Fire();
	}

	ReloadWhenEmpty();
}

void UCombatComponent::FinishReloading()
{
	if (!Character && !EquippedWeapon) return;

	int32 AmmountToReload = AmountToReload();
	if (WeaponAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		WeaponAmmoMap[EquippedWeapon->GetWeaponType()] -= AmmountToReload;
		CarriedAmmo = WeaponAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDCarriedWeaponAmmo(CarriedAmmo);
	}

	AddAmmo(AmmountToReload);

	CombatState = ECombatState::ECS_Unoccupied;
}

void UCombatComponent::ThrowGrenadeFinished()
{
	ShowAttachedGrenade(false);
	AttachActorToRightHand(EquippedWeapon);
	CombatState = ECombatState::ECS_Unoccupied;
}

void UCombatComponent::UpdateShotgunAmmoValues()
{
	if (!Character && !EquippedWeapon) return;

	if (EquippedWeapon->IsFull() || CarriedAmmo <= 0)
	{

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && Character->GetReloadMontage())
		{
			AnimInstance->Montage_JumpToSection(FName(TEXT("ShotgunEnd")));
		}

	}
	else
	{
		if (WeaponAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
		{
			WeaponAmmoMap[EquippedWeapon->GetWeaponType()] -= 1;
			CarriedAmmo = WeaponAmmoMap[EquippedWeapon->GetWeaponType()];
		}

		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			BlasterPlayerController->SetHUDCarriedWeaponAmmo(CarriedAmmo);
		}

		AddAmmo(1);
		bcanFire = true;
	}
	
}

void UCombatComponent::ShotgunShellReload()
{
	UpdateShotgunAmmoValues();
}

void UCombatComponent::InitializeCarriedAmmo()
{
	WeaponAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_SubmachineGun, StartingSubmachineAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingSniperRifleAmmo);
	WeaponAmmoMap.Emplace(EWeaponType::EWT_GranadeLauncher, StartingGranadeLauncherAmmo);

}

int32 UCombatComponent::AmountToReload()
{
	if (!EquippedWeapon) return 0;

	int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if (WeaponAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 Least = FMath::Min(RoomInMag, CarriedAmmo);
		return FMath::Clamp(RoomInMag, 0, Least);
	}

	return 0;
}

void UCombatComponent::AddAmmo(int32 AmmoToAdd)
{
	int32 NewAmmo = FMath::Clamp(EquippedWeapon->GetAmmo() + AmmoToAdd, 0, EquippedWeapon->GetMagCapacity());
	EquippedWeapon->SetAmmo(NewAmmo);
	EquippedWeapon->SetHUDAmmo();
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;
	DropEquippedWeapon();
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equip);

	AttachActorToRightHand(EquippedWeapon);

	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();

	UpdateCarryAmmo();
	PlayEquipWeaponSound(WeaponToEquip);
	ReloadWhenEmpty();
	
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquipSecondary);
	SecondaryWeapon->SetOwner(Character);

	PlayEquipWeaponSound(WeaponToEquip);
	AttachActorToBackpack(SecondaryWeapon);

}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (!Character || !WeaponToEquip) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (EquippedWeapon && !SecondaryWeapon)
	{
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else
	{
		EquipPrimaryWeapon(WeaponToEquip);
	}
	
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;

	
}

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
	if (Character && WeaponToEquip && WeaponToEquip->GetEquipSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponToEquip->GetEquipSound(), Character->GetActorLocation());
	}
}
void UCombatComponent::UpdateCarryAmmo()
{
	if (!EquippedWeapon) return;

	if (WeaponAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = WeaponAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : BlasterPlayerController;

	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDCarriedWeaponAmmo(CarriedAmmo);
	}
}

void UCombatComponent::UpdateThrowGrenades()
{
	if (!Character || !Character->Controller) return;

	BlasterPlayerController = (BlasterPlayerController == nullptr) ? (Cast<ABlasterPlayerController>(Character->Controller)) : (BlasterPlayerController);

	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDGrenade(CarriedGrenades);
	}
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if (!Character || !Character->GetMesh() || !ActorToAttach) return;
	bool bUsePistolSocket = 
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol ||
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SubmachineGun;

	FName SocketName = bUsePistolSocket ? FName(TEXT("WeaponSocket_R")) : FName(TEXT("WeaponSocket_R"));
	const USkeletalMeshSocket* RightHandSocket = Character->GetMesh()->GetSocketByName(SocketName);
	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
{
	if (!Character || !Character->GetMesh() || !ActorToAttach) return;

	bool bUsePistolSocket =
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol ||
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SubmachineGun;

	FName SocketName = bUsePistolSocket ? FName(TEXT("WeaponSocket_L")) : FName(TEXT("WeaponSocket_L"));

	const USkeletalMeshSocket* LeftHandSocket = Character->GetMesh()->GetSocketByName(SocketName);
	if (LeftHandSocket)
	{
		LeftHandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach)
{
	if (!Character || !Character->GetMesh() || !ActorToAttach) return;

	FName SocketName = FName(TEXT("BackpackSocket"));

	const USkeletalMeshSocket* BackpackSocket = Character->GetMesh()->GetSocketByName(SocketName);

	if (BackpackSocket)
	{
		BackpackSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::DropEquippedWeapon()
{
	if (EquippedWeapon)
	{
		Character->DropOrDestroyWeapon(EquippedWeapon);
		//EquippedWeapon->Dropped();
	}
}

void UCombatComponent::ReloadWhenEmpty()
{

	if (EquippedWeapon->GetAmmo() <= 0 && CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->Reload();
	}
}

void UCombatComponent::SetAiming(bool bAiming)
{
	if (!Character || !EquippedWeapon) return;

	isAiming = bAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bAiming ? AimWalkSpeed : BaseWalkSpeed;
	}

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		Character->ShowSniperScopeWidget(bAiming);
	}
}

void UCombatComponent::FireButtonPressed(bool bFirePressed)
{
	isFireButtonPressed = bFirePressed;
	if (!EquippedWeapon) return;

	if (Character && isFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (canFire())
	{
		bcanFire = false;
		Character->PlayFireMontage(isAiming);
		EquippedWeapon->Fire(HitTarget);

		if (EquippedWeapon)
		{
			CrosshairShootingFactor = 0.75f;
		}
		StartFireTimer();
	}
	
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;

	bool isScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, 
		CrossHairWorldPosition, 
		CrossHairWorldDirection);

	if (isScreenToWorld)
	{
		FVector Start = CrossHairWorldPosition;
		if (Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrossHairWorldDirection * (DistanceToCharacter + 100.f);
		}
		FVector End = Start + CrossHairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}

		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UCrosshairsInterface>())
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}

		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}

	}
}



