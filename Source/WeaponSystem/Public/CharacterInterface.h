// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

enum class EPlayerWeaponState : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WEAPONSYSTEM_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EPlayerWeaponState GetCurrentWeaponState() const = 0;
	virtual class AWeapon* GetCurrentWeapon() = 0;
	virtual void SetWeaponMember(class AWeapon* CurrentWeapon) = 0;
	virtual void SetWeaponState(EPlayerWeaponState State) = 0;
	virtual void TakeDamage(float ReceivedDamage) = 0;
};
