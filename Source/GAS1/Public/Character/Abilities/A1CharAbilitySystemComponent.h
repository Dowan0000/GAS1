// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "A1CharAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReveivedDamageDelegate, UA1CharAbilitySystemComponent*, SourceASC, float, UnmitigateDamage, float, MitigateDamage);

/**
 * 
 */
UCLASS()
class GAS1_API UA1CharAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReveivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UA1CharAbilitySystemComponent* SourceASC, float UnmitigateDamage, float MitigateDamage);
};
