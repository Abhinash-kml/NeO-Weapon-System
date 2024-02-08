// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Collider"));
	CollisionBox->SetupAttachment(WeaponMesh);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FString txt = FString::Printf(TEXT("Owner = %p"), GetOwner());
	//UKismetSystemLibrary::PrintString(this, txt);

	if (bRecoil)
	{
		VerticalRecoilTimeline.TickTimeline(DeltaTime);
		HorizontalRecoilTimeline.TickTimeline(DeltaTime);
		RecoilRecoveryTimeline.TickTimeline(DeltaTime);
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (LoadDataFrom == EDataLoadType::DataAsset)
		LoadFromDataAsset(DataAsset);
	else
		LoadFromDataTable(DataTable);
	
	TraceData.ScatteredEnd.AddDefaulted(ShotgunPallets);
	
	if (!CollisionBox->OnComponentBeginOverlap.IsBound())
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlap);

	if (bRecoil)
	{
		if (VerticalRecoilCurve)
		{
			FOnTimelineFloat VerticalRecoilProgressFunction;
			VerticalRecoilProgressFunction.BindUFunction(this, FName("HandleVerticalRecoilProgress"));
			VerticalRecoilTimeline.AddInterpFloat(VerticalRecoilCurve, VerticalRecoilProgressFunction);
		}
		if (HorizontalRecoilCurve)
		{
			FOnTimelineFloat HorizontalRecoilProgressFunction;
			HorizontalRecoilProgressFunction.BindUFunction(this, FName("HandleHorizontalRecoilProgress"));
			HorizontalRecoilTimeline.AddInterpFloat(HorizontalRecoilCurve, HorizontalRecoilProgressFunction);
		}
		if (RecoilRecoveryCurve)
		{
			FOnTimelineFloat RecoveryProgressFunction;
			RecoveryProgressFunction.BindUFunction(this, FName("HandleRecoveryProgress"));
			RecoilRecoveryTimeline.AddInterpFloat(RecoilRecoveryCurve, RecoveryProgressFunction);
		}
	}
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AWeapon::LoadFromDataAsset(UWeaponDataAsset* Asset)
{
	if (Asset->IsValidLowLevel())
	{
		bDrawDebug = Asset->bDrawDebug;
		DebugTime = Asset->DebugTime;
		WeaponType = Asset->WeaponType;
		FiringMode = Asset->FiringMode;
		TraceFrom = Asset->TraceFrom;
		TraceDistance = Asset->TraceDistance;
		bRecoil = Asset->bRecoil;
		bSpreadBullets = Asset->bSpreadBullets;
		SpreadFactor = Asset->SpreadFactor;
		bUnlimitedAmmo = Asset->bUnlimitedAmmo;
		CurrentClipAmmo = Asset->CurrentClipAmmo;
		MaxClipAmmo = Asset->MaxClipAmmo;
		BackpackAmmo = Asset->BackpackAmmo;
		WeaponFireRate = Asset->WeaponFireRate;
		BulletsPerBurst = Asset->BulletsPerBurst;
		ShotgunPallets = Asset->ShotgunPallets;
		TimeBetweenBurst = Asset->TimeBetweenBurst;
		WeaponMesh->SetSkeletalMesh(Asset->WeaponMesh);
		ArmSocket = Asset->ArmSocket;
		WeaponMuzzleSocket = Asset->WeaponMuzzleSocket;
		MuzzleFlashParticle = Asset->MuzzleFlashParticle;
		MuzzleFlashSystem = Asset->MuzzleFlashSystem;
		BulletImpactSystem = Asset->BulletImpactSystem;
		BulletImpactParticle = Asset->BulletImpactParticle;
		EquipAnimationStanding = Asset->EquipAnimationStanding;
		EquipAnimationCrouching = Asset->EquipAnimationCrouching;
		ShootAnimationStanding = Asset->ShootAnimationStanding;
		ShootAnimationCrouching = Asset->ShootAnimationCrouching;
		ReloadAnimationStanding = Asset->ReloadAnimationStanding;
		ReloadAnimationCrouching = Asset->ReloadAnimationCrouching;
		WeaponIcon = Asset->WeaponIcon;
		AmmoIcon = Asset->WeaponAmmoIcon;
		EquipSound = Asset->EquipSound;
		ShootSound = Asset->ShootSound;
		ReloadSound = Asset->ReloadSound;
		EmptyClipSound = Asset->EmptyClipSound;
		WeaponName = Asset->WeaponName;
		VerticalRecoilCurve = Asset->VerticalRecoilCurve;
		HorizontalRecoilCurve = Asset->HorizontalRecoilCurve;
		RecoilRecoveryCurve = Asset->RecoilRecoveryCurve;
		RecoilCameraShake = Asset->RecoilCameraShake;
	}
}
void AWeapon::LoadFromDataTable(FDataTableRowHandle RowHandle)
{
	if (RowHandle.DataTable)
	{
		FWeaponDataTable* Row = DataTable.GetRow<FWeaponDataTable>(TEXT("Weapon Data"));

		if (Row)
		{
			bDrawDebug = Row->bDrawDebug;
			DebugTime = Row->DebugTime;
			WeaponType = Row->WeaponType;
			FiringMode = Row->FiringMode;
			TraceFrom = Row->TraceFrom;
			TraceDistance = Row->TraceDistance;
			bRecoil = Row->bRecoil;
			bSpreadBullets = Row->bSpreadBullets;
			SpreadFactor = Row->SpreadFactor;
			bUnlimitedAmmo = Row->bUnlimitedAmmo;
			CurrentClipAmmo = Row->CurrentClipAmmo;
			MaxClipAmmo = Row->MaxClipAmmo;
			BackpackAmmo = Row->BackpackAmmo;
			WeaponFireRate = Row->WeaponFireRate;
			BulletsPerBurst = Row->BulletsPerBurst;
			ShotgunPallets = Row->ShotgunPallets;
			TimeBetweenBurst = Row->TimeBetweenBurst;
			WeaponMesh->SetSkeletalMesh(Row->WeaponMesh);
			ArmSocket = Row->ArmSocket;
			WeaponMuzzleSocket = Row->WeaponMuzzleSocket;
			MuzzleFlashParticle = Row->MuzzleFlashParticle;
			MuzzleFlashSystem = Row->MuzzleFlashSystem;
			BulletImpactSystem = Row->BulletImpactSystem;
			BulletImpactParticle = Row->BulletImpactParticle;
			EquipAnimationStanding = Row->EquipAnimationStanding;
			EquipAnimationCrouching = Row->EquipAnimationCrouching;
			ShootAnimationStanding = Row->ShootAnimationStanding;
			ShootAnimationCrouching = Row->ShootAnimationCrouching;
			ReloadAnimationStanding = Row->ReloadAnimationStanding;
			ReloadAnimationCrouching = Row->ReloadAnimationCrouching;
			WeaponIcon = Row->WeaponIcon;
			AmmoIcon = Row->WeaponAmmoIcon;
			EquipSound = Row->EquipSound;
			ShootSound = Row->ShootSound;
			ReloadSound = Row->ReloadSound;
			EmptyClipSound = Row->EmptyClipSound;
			WeaponName = Row->WeaponName;
			VerticalRecoilCurve = Row->VerticalRecoilCurve;
			HorizontalRecoilCurve = Row->HorizontalRecoilCurve;
			RecoilRecoveryCurve = Row->RecoilRecoveryCurve;
			RecoilCameraShake = Row->RecoilCameraShake;
		}
	}
}

void AWeapon::OnRep_WeaponOwner()
{
	
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeapon, WeaponOwner);
	DOREPLIFETIME(AWeapon, CurrentClipAmmo);
	DOREPLIFETIME(AWeapon, BackpackAmmo);
	DOREPLIFETIME(AWeapon, FxEmptyCounter);
	DOREPLIFETIME(AWeapon, bPickedUp);
	DOREPLIFETIME_CONDITION(AWeapon, TraceData, COND_SimulatedOnly);
}

void AWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ACharacter>() && !bPickedUp)
	{
		UKismetSystemLibrary::PrintString(this, "The overlapped actor is a Character");
		if (ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			//WeaponOwner = Character;
			SetOwner(Character);
			WeaponOwner = Character;
			bPickedUp = true;
			OnRep_Pickup();
			//ServerAttachToPlayer(Character);
		}
	}
}

void AWeapon::CalculateTraceStartAndEnd()
{
	FVector2D ViewPortSize;
	FVector2D CrosshairScreenLocation;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewPortSize);

	CrosshairScreenLocation = FVector2D(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(Cast<APlayerController>(WeaponOwner->GetController()), CrosshairScreenLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	//if (!bScreenToWorld)
		//UE_LOG(LogTemp, Warning, TEXT("DeprojectScreenToWorld() failed in %hs"), __FUNCTION__)
	//else
		//UE_LOG(LogTemp, Warning, TEXT("DeprojectScreenToWorld() passed in %hs"), __FUNCTION__)

	const float ConeHalfAngle = FMath::DegreesToRadians(SpreadFactor);
	
	// Lambda
	auto CalculateTraceEnd = [this, &ConeHalfAngle](const FVector_NetQuantize& Start, const FVector_NetQuantize& Direction) -> FVector_NetQuantize
	{
		if (bSpreadBullets)
		{
			const FVector_NetQuantize SpreadDirection = FMath::VRandCone(Direction, ConeHalfAngle, ConeHalfAngle);
			return Start + (SpreadDirection * TraceDistance);
		}
		else
			return Start + (Direction * TraceDistance);
	};
	
	switch (TraceFrom)
	{
		//using enum ETraceFrom;
	case ETraceFrom::ETF_Camera: {
			TraceData.Start = CrosshairWorldPosition;
			TraceData.End = CalculateTraceEnd(TraceData.Start, CrosshairWorldDirection);
			
			break;
	}
	case ETraceFrom::ETF_Muzzle: {
			if (const USkeletalMeshSocket* ValidSocket = WeaponMesh->GetSocketByName(FName("Muzzle")))
			{
				const FTransform SocketTransform = ValidSocket->GetSocketTransform(WeaponMesh);
				
				TraceData.Start = SocketTransform.GetLocation();
				TraceData.End = CalculateTraceEnd(TraceData.Start, CrosshairWorldDirection);
			}

			break;
	}
	default: { }
	}

	if (WeaponType == EWeaponType::EWT_Shotgun)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Size of ScatterArray = %i"), TraceData.ScatteredEnd.Num())

		for (int i = 0; i < ShotgunPallets; ++i)
			TraceData.ScatteredEnd[i] = CalculateTraceEnd(TraceData.Start, CrosshairWorldDirection);
	}
}

void AWeapon::ServerTraceAndDebug_Implementation(const FTraceData& TraceResult, const int& Bullets)
{
	const UWorld* const World = GetWorld();
	FHitResult OutHit;
	FVector_NetQuantize BeamEndPoint;
	TraceData = TraceResult;

	for (size_t i = 0; i < Bullets; ++i)
	{
		if (WeaponType == EWeaponType::EWT_Shotgun)
		{
			BeamEndPoint = TraceData.ScatteredEnd[i];
			World->LineTraceSingleByChannel(OutHit, TraceData.Start, TraceData.ScatteredEnd[i], ECollisionChannel::ECC_Pawn);
		}
		else
		{
			BeamEndPoint = TraceData.End;
			World->LineTraceSingleByChannel(OutHit, TraceData.Start, TraceData.End, ECollisionChannel::ECC_Pawn);
		}
        
        if (OutHit.bBlockingHit)
        {
	        BeamEndPoint = OutHit.Location;
        	if (AActor* HitActor = OutHit.GetActor())
        		if (ICharacterInterface* Interface = Cast<ICharacterInterface>(HitActor))
        			Interface->TakeDamage(20);
        }
        	

		if (bDrawDebug)
		{
			DrawDebugLine(World, TraceData.Start, BeamEndPoint, FColor::Cyan, false, DebugTime);
			DrawDebugPoint(World, BeamEndPoint, 10.f, FColor::Purple, false, DebugTime);
		}
	}

	ConsumeAmmo();
	CheckForReload();
}

void AWeapon::ShootingLogic()
{
	if (!WeaponOwner) return;

	if (IsClipEmpty())
	{
		WSFunctionLibrary::PlaySound(WeaponOwner, EmptyClipSound);
		ServerPlayEmptyFx();
		return;
	}
	
	if (FiringMode == EWeaponFiringType::EWFT_Automatic && !WeaponTimerHandle.IsValid())
		return;

	if (FiringMode == EWeaponFiringType::EWFT_Burst && !BurstTimerHandle.IsValid())
		return;
	
	if (FiringMode == EWeaponFiringType::EWFT_Burst)
	{
		if (BulletsFiredInBurst == BulletsPerBurst)
		{
			BulletsFiredInBurst = 0;
			GetWorldTimerManager().ClearTimer(BurstTimerHandle);

			if (bFireButtonReleased)
				return;

			FTimerDelegate Function;
			Function.BindUObject(this, &AWeapon::ReFire);
			GetWorldTimerManager().SetTimer(WeaponTimerHandle, Function, TimeBetweenBurst, false);
			return;
		}
		BulletsFiredInBurst++;
	}

	const UWorld* const World = GetWorld();
	FHitResult OutHit;
	
	//CrosshairLocation.Y -= 50.f
	
	FVector BeamEndPoint;
	const int BulletsToFire = WeaponType == EWeaponType::EWT_Shotgun ? ShotgunPallets : 1;

	for (int i = 0; i < BulletsToFire; ++i)
	{
		if (WeaponType == EWeaponType::EWT_Shotgun)
		{
			BeamEndPoint = TraceData.ScatteredEnd[i];
			World->LineTraceSingleByChannel(OutHit, TraceData.Start, TraceData.ScatteredEnd[i], ECollisionChannel::ECC_GameTraceChannel1);
		}
		else
		{
			BeamEndPoint = TraceData.End;
			World->LineTraceSingleByChannel(OutHit, TraceData.Start, TraceData.End, ECollisionChannel::ECC_GameTraceChannel1);
		}

		if (OutHit.bBlockingHit)
		{
			BeamEndPoint = OutHit.Location;

			if (!HasAuthority() && GetNetMode() == NM_Client)
				HandleBulletImpactCosmetics(OutHit.Location);
		}

		if (bDrawDebug)
		{
			DrawDebugLine(World, TraceData.Start, BeamEndPoint, FColor::Green, false, DebugTime);
			DrawDebugPoint(World, BeamEndPoint, 10.f, FColor::Red, false, DebugTime);
		}
	}

	if (!HasAuthority() && GetNetMode() == NM_Client)
	{
		HandleBulletFireCosmetics();
		
		if (bRecoil)
			Recoil();
	}

	ConsumeAmmo();
	CheckForReload();

	ServerTraceAndDebug(TraceData, BulletsToFire);
}

void AWeapon::ReFire()
{	
	GetWorldTimerManager().ClearTimer(BurstTimerHandle);
	FTimerDelegate Function;
	Function.BindUObject(this, &AWeapon::ShootingLogic);
	GetWorldTimerManager().SetTimer(BurstTimerHandle, Function, 0.1f, true, 0.001f);
}

void AWeapon::Recoil()
{
	const ACharacter* PlayerCharacter = WeaponOwner ? WeaponOwner : nullptr;
	APlayerController* CharacterController = Cast<APlayerController>(PlayerCharacter->GetController());

	if (CharacterController && ShotsFired > 0)
	{
		if (IsValid(VerticalRecoilCurve))
			CharacterController->AddPitchInput(VerticalRecoilCurve->GetFloatValue((60 / WeaponFireRate) * ShotsFired) * VerticalRecoilModifier);
		if (IsValid(HorizontalRecoilCurve))
			CharacterController->AddYawInput(HorizontalRecoilCurve->GetFloatValue((60 / WeaponFireRate) * ShotsFired) * HorizontalRecoilModifier);
	}
	else if (CharacterController && ShotsFired <= 0)
	{
		if (IsValid(VerticalRecoilCurve))
			CharacterController->AddPitchInput(VerticalRecoilCurve->GetFloatValue(0) * VerticalRecoilModifier);
		if (IsValid(HorizontalRecoilCurve))
			CharacterController->AddYawInput(HorizontalRecoilCurve->GetFloatValue(0) * HorizontalRecoilModifier);
	}

	ShotsFired++;
	CharacterController->ClientStartCameraShake(RecoilCameraShake);
}

void AWeapon::StartRecoil()
{
	const ACharacter* PlayerCharacter = WeaponOwner ? WeaponOwner : nullptr;
	const APlayerController* CharacterController = Cast<APlayerController>(PlayerCharacter->GetController());

	if (!IsClipEmpty() && CharacterController)
	{
		ControlRotation = CharacterController->GetControlRotation();
		bShouldRecover = true;
	}
}

void AWeapon::RecoilRecovery()
{
	if (bShouldRecover)
		RecoilRecoveryTimeline.PlayFromStart();
}

void AWeapon::HandleRecoveryProgress(float Value) const
{
	//UKismetSystemLibrary::PrintString(this, "RecoilRecovery()");
	const ACharacter* PlayerCharacter = WeaponOwner ? WeaponOwner : nullptr;
	APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());

	const FRotator NewControlRotation = FMath::Lerp(PlayerController->GetControlRotation(), ControlRotation, Value);

	PlayerController->SetControlRotation(NewControlRotation);
}

void AWeapon::HandleVerticalRecoilProgress(float Value) const
{
	const FString txt = FString::Printf(TEXT("Vertical Recoil = %f"), Value);
	UKismetSystemLibrary::PrintString(this, txt);
}

void AWeapon::HandleHorizontalRecoilProgress(float Value) const
{
	const FString txt = FString::Printf(TEXT("Horizontal Recoil = %f"), Value);
	UKismetSystemLibrary::PrintString(this, txt);
}

void AWeapon::ServerPlayEmptyFx_Implementation()
{
	FxEmptyCounter++;
}

void AWeapon::Shoot(float ElapsedTime)
{
	bFireButtonReleased = false;

	switch (FiringMode)
	{
		//using enum EWeaponFiringType;
	case EWeaponFiringType::EWFT_Single: {
			MidShoot(); 
			break; 
	}
	case EWeaponFiringType::EWFT_Burst: {
			FTimerDelegate Function;
			Function.BindUObject(this, &AWeapon::MidShoot);
			GetWorldTimerManager().SetTimer(BurstTimerHandle, Function, WeaponFireRate, true, 0.001f);
			break;
	}
	case EWeaponFiringType::EWFT_Automatic: { // Important: You need to set FirstDelay time, otherwise single taps on automatic weapon wont work properly
			FTimerDelegate Function;
			Function.BindUObject(this, &AWeapon::MidShoot);
			GetWorldTimerManager().SetTimer(WeaponTimerHandle, Function, WeaponFireRate, true, 0.001f);
			if (bRecoil) StartRecoil();
			break;
	}
	default: return;
	}
}

void AWeapon::MidShoot()
{
	CalculateTraceStartAndEnd();
	ShootingLogic();
}

void AWeapon::StopShoot()
{
	bFireButtonReleased = true;

	if (bRecoil)
	{
		VerticalRecoilTimeline.Stop();
		HorizontalRecoilTimeline.Stop();
		RecoilRecovery();
		ShotsFired = 0;
	} 

	if (WeaponTimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
	
	/* Never, ever reset BurstTimerHandle here, otherwise the burst will not complete completely */
}

void AWeapon::Reload()
{
	// Without this, game crashes when Reload is done on server
	if (!BackpackAmmo)
		return;
	
	if (CurrentClipAmmo == MaxClipAmmo)
		return;

	const int32 AmmoToAdd = MaxClipAmmo - CurrentClipAmmo;

	if (BackpackAmmo - AmmoToAdd <= 0) // This logic is same as - if (BackPackAmmo <= MaxClipAmmo) then add BackpackAmmo to CurrentClipAmmo and set CurrentClipAmmo to 0;
	{
		CurrentClipAmmo += BackpackAmmo;
		BackpackAmmo = 0;
	}
	else
	{
		BackpackAmmo -= AmmoToAdd;
		CurrentClipAmmo += AmmoToAdd;
	}
	
	const bool bIsCrouched = WeaponOwner->bIsCrouched;
	WSFunctionLibrary::PlayAnimation(WeaponOwner, bIsCrouched, ReloadAnimationStanding, ReloadAnimationCrouching);

	WSFunctionLibrary::PlaySound(WeaponOwner, ReloadSound);

	ServerReload();
}

void AWeapon::ServerReload_Implementation()
{
	Reload();
}

void AWeapon::ServerAttachToPlayer_Implementation(ACharacter* Player)
{
	bPickedUp = true;
	WeaponOwner = Player;
	OnRep_Pickup();
	OnRep_WeaponOwner();
}

void AWeapon::HandleAttachToPlayer(ACharacter* Player)
{
	if (Player)
	{
		if (ICharacterInterface* Interface = Cast<ICharacterInterface>(Player))
		{
			Interface->SetWeaponMember(this);
			Interface->SetWeaponState(EPlayerWeaponState::Rifle);

			// On server
			WeaponOwner = Player;
			
			if (GetOwner() != Player)
				SetOwner(Player);

			if (const USkeletalMeshSocket* ValidSocket = Player->GetMesh()->GetSocketByName(ArmSocket))
			{
				WeaponMesh->SetSimulatePhysics(false);
				WeaponMesh->SetEnableGravity(false);
				WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

				/* Note to self 
				* The result of Activating and Deactiving CollisionBox by using SetActive()
				* is same as Activing and Deactivating CollisionBox's OverlapEvent.
				* So we can use any of the one, here triggering overlap event is used      */
				CollisionBox->SetGenerateOverlapEvents(false);
				/* This method is disabled for now,  ut the effect is same */
				//CollisionBox->SetActive(false);

				WeaponMesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ArmSocket);
				//AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ArmSocket);
			}
		}

		bool bIsCrouched = Player->bIsCrouched;
		WSFunctionLibrary::PlayAnimation(Player, bIsCrouched, EquipAnimationStanding, EquipAnimationCrouching);

		WSFunctionLibrary::PlaySound(Player, EquipSound);

		UKismetSystemLibrary::PrintString(this, "Weapon attched to character");
	}
}


void AWeapon::ServerDetachFromPlayer_Implementation()
{
	if (HasAuthority())
	{
		bPickedUp = false;
		OnRep_Pickup();
	}
}

void AWeapon::HandleDetach()
{
	if (WeaponOwner)
	{
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		ICharacterInterface* Interface = Cast<ICharacterInterface>(WeaponOwner);
		if (Interface)
		{
			Interface->SetWeaponMember(nullptr);
			Interface->SetWeaponState(EPlayerWeaponState::NoWeapon);
		}

		if (HasAuthority())
			UKismetSystemLibrary::PrintString(this, "Ran on server");

		WeaponOwner = nullptr;
		SetOwner(nullptr);

		/* We set a 2 secs delay and they enable overlap of the box collider
		else the player cannot drop the weapon. Since enabling overlap event
		just after dropping reattaches the weapon to the player */
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AWeapon::ResetWeaponCollision, 0.2f, false);
	}
}


void AWeapon::ResetWeaponCollision()
{
	/* Note to self
	* The result of Activating and Deactiving CollisionBox by using SetActive()
	* is same as Activing and Deactivating CollisionBox's OverlapEvent.
	* So we can use any of the one, here triggering overlap event is used      */
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	/* This method is disabled for now, but the effect is same */
	//CollisionBox->SetActive(true);
}

void AWeapon::HandleBulletImpactCosmetics(FVector TraceHitPoint) const
{
	UWorld* World = GetWorld();
	const FTransform Transform = UKismetMathLibrary::MakeTransform(TraceHitPoint, FRotator::ZeroRotator, FVector::OneVector);

	if (bUsingNiagara && IsValid(BulletImpactSystem))
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, BulletImpactSystem, Transform.GetLocation());
	else if (IsValid(BulletImpactParticle))
		UGameplayStatics::SpawnEmitterAtLocation(World, BulletImpactParticle, Transform);
}

void AWeapon::HandleBulletFireCosmetics() const
{
	UWorld* World = GetWorld();
	const bool bIsCrouched = WeaponOwner->bIsCrouched;

	WSFunctionLibrary::PlaySound(WeaponOwner, ShootSound);

	WSFunctionLibrary::PlayAnimation(WeaponOwner, bIsCrouched, ShootAnimationStanding, ShootAnimationCrouching);
	
	if (const USkeletalMeshSocket* ValidSocket = WeaponMesh->GetSocketByName(WeaponMuzzleSocket))
	{
		const FTransform SocketTransform = ValidSocket->GetSocketTransform(WeaponMesh);
		if (bUsingNiagara && IsValid(MuzzleFlashSystem))
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, MuzzleFlashSystem, SocketTransform.GetLocation());
		else if (IsValid(MuzzleFlashParticle))
			UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlashParticle, SocketTransform);
	} 
}

bool AWeapon::IsClipEmpty() const
{
	if (CurrentClipAmmo <= 0)
		return true;
	
	return false;
}

void AWeapon::CheckForReload()
{
	if (IsClipEmpty())
		if (BackpackAmmo)
			Reload();
}

void AWeapon::ConsumeAmmo()
{
	//if (bUnlimitedAmmo)
		//return;

	if (HasAuthority())
	{
		FString txt = FString::Printf(TEXT("ConsumeAmmo in server"));
		UKismetSystemLibrary::PrintString(this, txt);
	}
	
	//FxCounter++;
	CurrentClipAmmo--;
}

void AWeapon::OnRep_Pickup()
{
	UKismetSystemLibrary::PrintString(this, "PickupCalled on client");
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bPickedUp)
			HandleAttachToPlayer(WeaponOwner);
		else
			HandleDetach();
	}
}

void AWeapon::OnRep_FxEmptyCounter()
{
	UKismetSystemLibrary::PrintString(this, "Clip Empty sound played on clients");
	WSFunctionLibrary::PlaySound(WeaponOwner, EmptyClipSound, true);
}

void AWeapon::OnRep_ClipAmmo()
{
	HandleBulletFireCosmetics();
	UKismetSystemLibrary::PrintString(this, "ClipAmmo Replicated");
}

void AWeapon::OnRep_TraceData()
{
	// Nothing here for now
}

#if WITH_EDITOR
bool AWeapon::CanEditChange(const FProperty* InProperty) const
{
	// If other logic prevents editing, we want to respect that
	const bool ParentVal = Super::CanEditChange(InProperty);

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AWeapon, FiringMode))
	{
		switch (WeaponType)
		{
			case EWeaponType::EWT_Rifle: return ParentVal && true;
			case EWeaponType::EWT_Pistol: return ParentVal && true;
			case EWeaponType::EWT_Grenade: return ParentVal && false;
			case EWeaponType::EWT_Melee: return ParentVal && false;
		}
	}

	return ParentVal;
}
#endif
