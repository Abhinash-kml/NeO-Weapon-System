// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\WSFunctionLibrary.h"

WSFunctionLibrary::WSFunctionLibrary()
{
}

void WSFunctionLibrary::PlayAnimation(ACharacter* Player, bool bIsCrouched, UAnimMontage* StandingMontage, UAnimMontage* CrouchedMontage, FName Section)
{
	//if (!Player)
		//UE_LOG(LogTemp, Fatal, TEXT("Supplied Character in %hs is not valid"), __FUNCTION__);

	if (!StandingMontage)
		UE_LOG(LogTemp, Warning, TEXT("Supplied Animation %hs is not valid"), __FUNCTION__);

	if (!CrouchedMontage)
		UE_LOG(LogTemp, Warning, TEXT("Supplied Animation %hs is not valid"), __FUNCTION__);

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (!bIsCrouched)
		{
			AnimInstance->Montage_Play(StandingMontage);
			if (!Section.IsEqual(FName("Default")))
				AnimInstance->Montage_JumpToSection(Section);
		}
		else
		{
			AnimInstance->Montage_Play(CrouchedMontage);
			if (!Section.IsEqual(FName("Default")))
				AnimInstance->Montage_JumpToSection(Section);
		}
	}
}

void WSFunctionLibrary::PlaySound(ACharacter* Player, USoundCue* SoundCue, bool bAtLocation)
{
	//if (!Player)
		//UE_LOG(LogTemp, Fatal, TEXT("The Character supplied in %hs is not valid"), __FUNCTION__);

	if (!SoundCue) 
		UE_LOG(LogTemp, Warning, TEXT("The SoundCue supplied in %hs is not valid"), __FUNCTION__);
	
	UWorld* World = Player->GetWorld();

	if (!bAtLocation)
	{
		UGameplayStatics::PlaySound2D(World, SoundCue);
		return;
	}

	FVector Location = Player->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(World, SoundCue, Location);
}

WSFunctionLibrary::~WSFunctionLibrary()
{
}
