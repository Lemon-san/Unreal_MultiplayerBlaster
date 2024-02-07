// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/BlasterTypes/DebugMacros.h"


void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	if (!BlasterCharacter) return;

	BlasterMovement = BlasterCharacter->GetCharacterMovement();

}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}

	if (!BlasterCharacter) return;

	Speed = UKismetMathLibrary::VSizeXY(BlasterMovement->Velocity);
	isFalling = BlasterMovement->IsFalling();
	isAccelerating = BlasterMovement->GetCurrentAcceleration().Size() > 0.f;
	isWeaponEquipped = BlasterCharacter->isWeaponEquipped();
	WeaponEquipped = BlasterCharacter->GetEquippedWeapon();
	isCrouched = BlasterCharacter->bIsCrouched;
	isAiming = BlasterCharacter->isCharacterAiming();
	TurningInPlace = BlasterCharacter->GetTurningInplace();
	isElimmed = BlasterCharacter->GetisElimmed();
	
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BlasterCharacter->GetActorRotation();
	
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float LeanInterp= FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(LeanInterp, -90.f, 90.f);
	
	AimOffsetYaw = BlasterCharacter->GetHipAimOffsetYaw();
	AimOffsetPitch = BlasterCharacter->GetHipAimOffsetPitch();

	if (isWeaponEquipped && WeaponEquipped && WeaponEquipped->GetWeaponMesh() && BlasterCharacter->GetMesh())
	{
		FRotator OutRotation;
		FVector OutPosition;
		LeftHandTransform = WeaponEquipped->GetWeaponMesh()->GetSocketTransform(FName(TEXT("LeftHandSocket")), ERelativeTransformSpace::RTS_World);
		BlasterCharacter->GetMesh()->TransformToBoneSpace(FName(TEXT("hand_r")), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		FTransform RightHandTransform = WeaponEquipped->GetWeaponMesh()->GetSocketTransform(FName(TEXT("Hand_R")), ERelativeTransformSpace::RTS_World);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget()));
		RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);

		FTransform MuzzleTipTransform = WeaponEquipped->GetWeaponMesh()->GetSocketTransform(FName(TEXT("MuzzleFlash")), ERelativeTransformSpace::RTS_World);
		FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));

	}

	bUseFABRIK = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	bUseAimOffsets = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	bTransformRightHand = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;

}
