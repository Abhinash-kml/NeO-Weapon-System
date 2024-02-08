// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

/**
 * 
 */
class WEAPONSYSTEM_API WSFunctionLibrary
{
public:
	WSFunctionLibrary();

	static void PlayAnimation(ACharacter* Player, bool bIsCrouched, UAnimMontage* StandingMontage, UAnimMontage* CrouchedMontage, FName Section = FName("Default"));
	static void PlaySound(ACharacter* Player, USoundCue* SoundCue, bool bAtLocation = false);

	~WSFunctionLibrary();
};
