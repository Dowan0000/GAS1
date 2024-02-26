// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/A1PlayerState.h"
#include "Character/Abilities/A1CharAbilitySystemComponent.h"


AA1PlayerState::AA1PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UA1CharAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UA1CharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	NetUpdateFrequency = 100.f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

void AA1PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AA1PlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AA1PlayerState::MaxHealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AA1PlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AA1PlayerState::MaxManaChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AA1PlayerState::CharacterLevelChanged);
	
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AA1PlayerState::StunTagChanged);
	}
}

UAbilitySystemComponent* AA1PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UA1CharacterAttributeSet* AA1PlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}

bool AA1PlayerState::IsAlive() const
{
	return GetHealth() > 0.f;
}

void AA1PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	//TODO HUD later
}

float AA1PlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AA1PlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AA1PlayerState::GetMana() const
{
	return AttributeSet->GetMana();
}

float AA1PlayerState::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

int32 AA1PlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetLevel();
}

void AA1PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}

void AA1PlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed!"));
}

void AA1PlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Mana Changed!"));
}

void AA1PlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Mana Changed!"));
}

void AA1PlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Level Changed!"));
}

void AA1PlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	
		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));
		
		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);

	}
}
