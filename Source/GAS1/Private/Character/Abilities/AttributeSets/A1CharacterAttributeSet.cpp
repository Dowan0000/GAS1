// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/A1CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UA1CharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UA1CharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UA1CharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UA1CharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UA1CharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UA1CharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UA1CharacterAttributeSet, Health, OldHealth);
}

void UA1CharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UA1CharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UA1CharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UA1CharacterAttributeSet, Mana, OldMana);
}

void UA1CharacterAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UA1CharacterAttributeSet, MaxMana, OldMaxMana);
}
