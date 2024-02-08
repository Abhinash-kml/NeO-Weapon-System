// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "WSFunctionLibrary.h"
#include "WSTypes.h"
#include "Camera/CameraShakeBase.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/Texture2D.h"
#include "Gameframework/Actor.h"
#include "Gameframework/Character.h"
#include "Gameframework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Weapon.generated.h"

class UNiagaraSystem;

/*/UENUM()
enum class WeaponState : uint8
{
	OnGround,	// Not picked up
	Equipped,	// Picked up but not in hand
	Active,		// Picked up and in hand
	Reloading	// Picked up, in hand and reloading
};*/

/**
 * Weapon Master Class
 */
UCLASS()
class WEAPONSYSTEM_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float Deltatime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Applies the actual Recoil */
	void Recoil();

	/** Begins applying recoil to the weapon */
	void StartRecoil();

	/** Initiates the recoil function */
	void RecoilRecovery();

	/** Interpolates the player back to their initial view vector */
	UFUNCTION()
	void HandleRecoveryProgress(float Value) const;

	UFUNCTION()
	void HandleVerticalRecoilProgress(float Value) const;

	UFUNCTION()
	void HandleHorizontalRecoilProgress(float Value) const;
	
	void CalculateTraceStartAndEnd();
	
	UFUNCTION(Server, Unreliable)
	void ServerTraceAndDebug(const FTraceData& TraceResult, const int& Bullets);

	UFUNCTION(Server, Unreliable)
	void ServerPlayEmptyFx();

public:
	// Custom functions

	/** Start Shooting the current Weapon */
	UFUNCTION(BlueprintCallable)
	void Shoot(float ElapsedTime);
	
	void MidShoot();
	
	/** Stop Shooting the current Weapon */
	UFUNCTION(BlueprintCallable)
	void StopShoot();
	
	/** Reloads the current Weapon */
	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	/** The main logic behind shooting i.e. LineTrace and Other Things */
	void ShootingLogic();

	/** Refires the Burst Weapon */
	void ReFire();
	
	void HandleAttachToPlayer(ACharacter*);
	
	/** Attach the Weapon to the player */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAttachToPlayer(ACharacter* Player);

	/** Detach the Weapon from the player */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDetachFromPlayer();

	void HandleDetach();
	
	/** Resets dropped Weapon's Collision */
	void ResetWeaponCollision();

	/** [Called Locally] */
	void HandleBulletImpactCosmetics(FVector HitLocation) const;

	/** [Called Locally] */
	void HandleBulletFireCosmetics() const;

	/** Checks if current Magazine is empty or not */
	bool IsClipEmpty() const;

	/** Checks if current Weapon should be reloaded */
	void CheckForReload();

	/** Consume ammo for the current Weapon */
	void ConsumeAmmo();

	/** Get current ammo in the magazine */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentClipAmmo() const { return CurrentClipAmmo; }

	/** Get max ammo that current Weapon's magazine can contain */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxClipAmmo() const { return MaxClipAmmo; }

	/** Get ammo contained in player's backpack */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetBackpackAmmo() const { return BackpackAmmo; }

	/** Load Data from Data Asset */
	void LoadFromDataAsset(UWeaponDataAsset* Asset);

	/** LoadData from DataTable */
	void LoadFromDataTable(FDataTableRowHandle RowHandle);

	/* Important for Hiding Property Values depending on other property 
	Its a necessary to build it with WITH_EDITOR preprocessor else it wont work */
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

private:
	UPROPERTY(ReplicatedUsing=OnRep_TraceData)
	FTraceData TraceData;
	
	UPROPERTY(ReplicatedUsing=OnRep_Pickup)
	bool bPickedUp;

	UPROPERTY(ReplicatedUsing=OnRep_FxEmptyCounter)
	int FxEmptyCounter;

	UFUNCTION()
	void OnRep_TraceData();

	UFUNCTION()
	void OnRep_Pickup();

	UFUNCTION()
	void OnRep_FxEmptyCounter();
	
	/** Where to load Data from */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Configuration",
		meta = (DisplayPriority = -1, AllowPrivateAccess = "true"))
	EDataLoadType LoadDataFrom = EDataLoadType::Blueprint;

	/* DataTable for Weapon Generation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Configuration",
		meta = (EditCondition = "LoadDataFrom == EDataLoadType::DataTable", AllowPrivateAccess = "true"))
	FDataTableRowHandle DataTable;

	/** DataAsset for Weapon Generation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Configuration",
		meta = (EditCondition = "LoadDataFrom == EDataLoadType::DataAsset", AllowPrivateAccess = "true"))
	UWeaponDataAsset* DataAsset;

	/** For drawing debug */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	/** Debug time */
	UPROPERTY()
	float DebugTime;

	/** Weapon Type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	/** Weapon firing mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponFiringType FiringMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	ETraceFrom TraceFrom = ETraceFrom::ETF_Camera;
	/** The Distance to trace from starting point */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float TraceDistance;

	/** Should the Weapon have Recoil */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bRecoil;

	/** Should the Weapon have Bullet Spread */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bSpreadBullets;

	/** The Spread Factor for Bullet Spread */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float SpreadFactor;

	/** Should the Weapon have Unlimited Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bUnlimitedAmmo;

	/** Ammo in the current Magazine */
	UPROPERTY(ReplicatedUsing=OnRep_ClipAmmo)
	int CurrentClipAmmo;

	UFUNCTION()
	void OnRep_ClipAmmo();

	/** Maximum Ammo that current Weapon's Magazine can contain */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int MaxClipAmmo;

	/** Ammo in backpack that player contains */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int BackpackAmmo;

	/** Fire rate of the Current Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float WeaponFireRate;

	/** Bullets per Burst of the Current Burst Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int BulletsPerBurst;

	/** The Pallets of a Current Weapon if its a Shotgun */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Iterm Properties", meta = (AllowPrivateAccess = "true"))
	int ShotgunPallets;

	/** The Delay between each Burst of the Current Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenBurst;

	/* Skeletal Mesh of Current Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	/* Box collider for equipping and showing hud for this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	/* Skeletal Mesh of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName ArmSocket;

	/* Skeletal Mesh of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName WeaponMuzzleSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bUsingNiagara;

	/** Muzzle flash of the Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlashParticle;

	/** Impact Particle of the Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* BulletImpactParticle;

	/** Niagara Muzzle flash particle of the Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* MuzzleFlashSystem;

	/** Niagara Impact particle of the Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* BulletImpactSystem;

	/* Weapon Name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString WeaponName = "Default";

	/* Weapon Equip Animation while standing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* EquipAnimationStanding;

	/* Weapon Equip Animation while sitting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* EquipAnimationCrouching;

	/* Weapon Shoot Animation while standing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ShootAnimationStanding;

	/* Weapon Shoot Animation while crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ShootAnimationCrouching;

	/** Weapon Reload Animation while standing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ReloadAnimationStanding;

	/** Weapon Reload Animation while crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ReloadAnimationCrouching;

	/** Weapon Icon for HUD */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Icon", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* WeaponIcon;

	/** Weapon Ammo Icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Icon", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* AmmoIcon;

	/** Weapon equip Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sound", meta = (AllowPrivateAccess = "true"))
	class USoundCue* EquipSound;

	/** Weapon shoot Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sound", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShootSound;

	/** Weapon reload Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sound", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ReloadSound;

	/** Weapon empty clip Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sound", meta = (AllowPrivateAccess = "true"))
	class USoundCue* EmptyClipSound;

	/** Current owner of this Weapon */
	UPROPERTY(ReplicatedUsing=OnRep_WeaponOwner, BlueprintReadOnly, Category = "Weapon Main", meta = (AllowPrivateAccess = "true"))
	class ACharacter* WeaponOwner = nullptr;

	UFUNCTION()
	void OnRep_WeaponOwner();

	/** Timer handle for Automatic Fire */
	FTimerHandle WeaponTimerHandle;

	/** Timer handle for Burst Fire */
	FTimerHandle BurstTimerHandle;

	/** Timer handle for predicting and playing FX locally */
	FTimerHandle LocalFxTimerHandle;

	/** Counter for bullets fired in a burst */
	int BulletsFiredInBurst = 0;

	/** Fire button down status */
	bool bFireButtonReleased;

	// ============================================================================================
	// Recoil Stuffs
	// ============================================================================================

	/** The curve for vertical recoil (set from WeaponData) */
	UPROPERTY()
	UCurveFloat* VerticalRecoilCurve;

	/** The timeline for vertical recoil (generated from the curve) */
	FTimeline VerticalRecoilTimeline;

	/** The curve for horizontal recoil (set from WeaponData) */
	UPROPERTY()
	UCurveFloat* HorizontalRecoilCurve;

	/** The timeline for horizontal recoil (generated from the curve) */
	FTimeline HorizontalRecoilTimeline;

	/** The curve for Recoil recovery */
	UPROPERTY()
	UCurveFloat* RecoilRecoveryCurve;

	/** The timeline for recover (set from the curve) */
	FTimeline RecoilRecoveryTimeline;

	/** A value to temporarily cache the player's control rotation so that we can return to it */
	FRotator ControlRotation;

	/** Keeping track of whether we should do a recoil recovery after finishing firing or not */
	bool bShouldRecover;

	/** Used in recoil to make sure the first shot has properly applied recoil */
	int ShotsFired;

	/** The base multiplier for vertical recoil, modified by attachments */
	float VerticalRecoilModifier = 1.0f;

	/** The base multiplier for horizontal recoil, modified by attachments */
	float HorizontalRecoilModifier = 1.0f;

	/** The Camera Shake class to use for Camera Shake during recoil */
	UPROPERTY()
	TSubclassOf<UCameraShakeBase> RecoilCameraShake;


public:
	UFUNCTION()
	class UBoxComponent* GetCollider() const { return CollisionBox; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
