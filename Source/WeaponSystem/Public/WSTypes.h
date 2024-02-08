// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WSTypes.generated.h"

UENUM()
enum class EDataLoadType : uint8
{
	Blueprint UMETA(DisplayName = "This Blueprint"),
	DataAsset UMETA(DisplayName = "Data Asset"),
	DataTable UMETA(DisplayName = "Data Table")
};

UENUM(BlueprintType)
enum class EPlayerWeaponState : uint8
{
	NoWeapon UMETA(DisplayName = "Weapon Less"),
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName = "Pistol"),
	Melee UMETA(DisplayName = "Melee")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_Rifle UMETA(DisplayName = "Rifle"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_Melee UMETA(DisplayName = "Melee"),
	EWT_Grenade UMETA(DisplayName = "Grenade")
};

UENUM(BlueprintType)
enum class EWeaponFiringType : uint8
{
	EWFT_Single UMETA(DisplayName = "Single Shot"),
	EWFT_Burst UMETA(DisplayName = "Burst Shot"),
	EWFT_Automatic UMETA(DisplayName = "Automatic Shot")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Idle UMETA(DisplayName = "Idle"),
	EWS_Firing UMETA(DisplayName = "Firing"),
	EWS_Reloading UMETA(DisplayName = "Reloading")
};

UENUM(BlueprintType)
enum class EWeaponWorldState : uint8
{
	EWWS_OnGround UMETA(DisplayName = "On Ground"),
	EWWS_Attached UMETA(DisplayName = "Attached To Player")
};

UENUM(BlueprintType)
enum class ETraceFrom : uint8
{
	ETF_Camera UMETA(DisplayName = "Camera"),
	ETF_Muzzle UMETA(DisplayName = "Muzzle")
};

UCLASS()
class UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Debug", meta = (DisplayPriority = 1))
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Debug", meta = (DisplayPriority = 1))
	float DebugTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	FString WeaponName = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	EWeaponFiringType FiringMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	ETraceFrom TraceFrom = ETraceFrom::ETF_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bSpreadBullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details",
		meta = (EditCondition = "bSpreadBullets == true", EditConditionHides, DisplayPriority = 2))
	float SpreadFactor = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bUnlimitedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int CurrentClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int MaxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int BackpackAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float WeaponFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int BulletsPerBurst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details",
		meta = (EditCondition = "WeaponType == EWeaponType::EWT_Shotgun", EditConditionHides, DisplayPriority = 2))
	int ShotgunPallets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float TimeBetweenBurst;

	/** The curve for vertical recoil (set from WeaponData) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	UCurveFloat* VerticalRecoilCurve;

	/** The curve for horizontal recoil (set from WeaponData) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	UCurveFloat* HorizontalRecoilCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	UCurveFloat* RecoilRecoveryCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	TSubclassOf<UCameraShakeBase> RecoilCameraShake;

	/** The base multiplier for vertical recoil, modified by attachments */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	float VerticalRecoilModifier = 1.0f;

	/** The base multiplier for horizontal recoil, modified by attachments */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil", meta = (DisplayPriority = 3))
	float HorizontalRecoilModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 4))
	class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 4))
	FName ArmSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 4))
	FName WeaponMuzzleSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 5))
	bool bUsingNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 5, EditCondition = "bUsingNiagara == false"))
	class UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 5, EditCondition = "bUsingNiagara == false"))
	class UParticleSystem* BulletImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 5, EditCondition = "bUsingNiagara == true"))
	class UNiagaraSystem* MuzzleFlashSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 5, EditCondition = "bUsingNiagara == true"))
	class UNiagaraSystem* BulletImpactSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 6))
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 6))
	class USoundCue* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 6))
	class USoundCue* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 6))
	class USoundCue* EmptyClipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* EquipAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* EquipAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* ShootAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* ShootAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* ReloadAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 7))
	class UAnimMontage* ReloadAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|HUD", meta = (DisplayPriority = 8))
	class UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|HUD", meta = (DisplayPriority = 8))
	class UTexture2D* WeaponAmmoIcon;

public:
#if WITH_EDITOR
	bool CanEditChange(const FProperty* InProperty) const override;
#endif
};

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Debug", meta = (DisplayPriority = 1))
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Debug", meta = (DisplayPriority = 1))
	float DebugTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	FString WeaponName = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	EWeaponFiringType FiringMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	ETraceFrom TraceFrom = ETraceFrom::ETF_Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bSpreadBullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details",
		meta = (EditCondition = "bSpreadBullets == true", EditConditionHides, DisplayPriority = 2))
	float SpreadFactor = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	bool bUnlimitedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int CurrentClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int MaxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int BackpackAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float WeaponFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	int BulletsPerBurst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Details",
		meta = (EditCondition = "WeaponType == EWeaponType::EWT_Shotgun", EditConditionHides, DisplayPriority = 2))
	int ShotgunPallets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Details", meta = (DisplayPriority = 2))
	float TimeBetweenBurst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 3))
	class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 3))
	FName ArmSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh", meta = (DisplayPriority = 3))
	FName WeaponMuzzleSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 4))
	bool bUsingNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 4))
	class UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 4))
	class UParticleSystem* BulletImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 4))
	class UNiagaraSystem* MuzzleFlashSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visual Effects", meta = (DisplayPriority = 4))
	class UNiagaraSystem* BulletImpactSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 5))
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 5))
	class USoundCue* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 5))
	class USoundCue* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound", meta = (DisplayPriority = 5))
	class USoundCue* EmptyClipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* EquipAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* EquipAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* ShootAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* ShootAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* ReloadAnimationStanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation", meta = (DisplayPriority = 6))
	class UAnimMontage* ReloadAnimationCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|HUD", meta = (DisplayPriority = 7))
	class UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|HUD", meta = (DisplayPriority = 7))
	class UTexture2D* WeaponAmmoIcon;

	/** The curve for vertical recoil (set from WeaponData) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	UCurveFloat* VerticalRecoilCurve;

	/** The curve for horizontal recoil (set from WeaponData) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	UCurveFloat* HorizontalRecoilCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	UCurveFloat* RecoilRecoveryCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	TSubclassOf<UCameraShakeBase> RecoilCameraShake;

	/** The base multiplier for vertical recoil, modified by attachments */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	float VerticalRecoilModifier = 1.0f;

	/** The base multiplier for horizontal recoil, modified by attachments */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Miscellaneous", meta = (DisplayPriority = 8))
	float HorizontalRecoilModifier = 1.0f;
};

USTRUCT()
struct FTraceData final
{
	GENERATED_BODY()

	FTraceData() = default;
	FTraceData(const FTraceData& Other)
		: Start(Other.Start)
		, End(Other.End)
		, ScatteredEnd(Other.ScatteredEnd)
	{ }

	FTraceData& operator=(const FTraceData& Other)
	{
		if (&Other == this)
			return *this;

		Start = Other.Start;
		End = Other.End;
		ScatteredEnd = Other.ScatteredEnd;

		return *this;
	}

	FTraceData(FTraceData&& Other) = default;
	FTraceData& operator=(FTraceData&& Other) = default;

	~FTraceData() = default;

	UPROPERTY()
	FVector_NetQuantize Start;

	UPROPERTY()
	FVector_NetQuantize End;

	UPROPERTY()
	TArray<FVector_NetQuantize> ScatteredEnd;
};
