// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/CharacterTypes.h"
#include "BlasterAnimInstance.generated.h"


class UCharacterMovementComponent;
class ABlasterCharacter;
class AWeapon;

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FRotator CharacterRotationLastFrame;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isAccelerating = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* BlasterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool isCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool isWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeapon* WeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool isAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float AimOffsetYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float AimOffsetPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Types")
	ETurningInPlace TurningInPlace;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool isElimmed;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bUseFABRIK;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bUseAimOffsets;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bTransformRightHand;


};
