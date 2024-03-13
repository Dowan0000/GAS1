// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/A1PlayerController.h"
#include "Player/A1PlayerState.h"
#include "AbilitySystemComponent.h"

void AA1PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AA1PlayerState* PS = GetPlayerState<AA1PlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

// TODO -- Add HUD Stuff