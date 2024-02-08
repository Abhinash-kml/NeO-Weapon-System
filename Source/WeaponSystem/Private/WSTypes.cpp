// Fill out your copyright notice in the Description page of Project Settings.


#include "WSTypes.h"


#if WITH_EDITOR
bool UWeaponDataAsset::CanEditChange(const FProperty* InProperty) const
{
	// If other logic prevents editing, we want to respect that
	const bool ParentVal = Super::CanEditChange(InProperty);

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, TraceDistance))
	{
		switch (WeaponType)
		{
			case EWeaponType::EWT_Grenade: return ParentVal && false;
			case EWeaponType::EWT_Melee: return ParentVal && false;
			default: return ParentVal && true;
		}
	}

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, FiringMode))
	{
		switch (WeaponType)
		{
			case EWeaponType::EWT_Grenade: return ParentVal && false;
			case EWeaponType::EWT_Melee: return ParentVal && false;
			case EWeaponType::EWT_Shotgun: return ParentVal && false;
			default: return ParentVal && true;
		}
	}

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, bUnlimitedAmmo))
	{
		switch (WeaponType)
		{
			case EWeaponType::EWT_Melee: return ParentVal && false;
			default: return ParentVal && true;
		}
	}

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, WeaponFireRate))
	{
		switch (WeaponType)
		{
			case EWeaponType::EWT_Grenade: return ParentVal && false;
			case EWeaponType::EWT_Melee: return ParentVal && false;
			default: return ParentVal && true;
		}
	}

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, BulletsPerBurst)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UWeaponDataAsset, TimeBetweenBurst))
	{
		bool bInner;

		switch (WeaponType)
		{
			case EWeaponType::EWT_Grenade: { bInner = false; break; }
			case EWeaponType::EWT_Melee: { bInner = false; break; }
			case EWeaponType::EWT_Shotgun: { bInner = false; break; }
			default: bInner = true;
		}

		if (FiringMode == EWeaponFiringType::EWFT_Burst)
			return (bInner && true);
		else
			return (bInner && false);
	}

	return ParentVal;
}
#endif
