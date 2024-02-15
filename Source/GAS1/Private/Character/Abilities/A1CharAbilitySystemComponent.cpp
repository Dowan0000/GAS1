// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/A1CharAbilitySystemComponent.h"

void UA1CharAbilitySystemComponent::ReceiveDamage(UA1CharAbilitySystemComponent* SourceASC, float UnmitigateDamage, float MitigateDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigateDamage, MitigateDamage);
}
