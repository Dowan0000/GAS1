// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAS1Character.h"
#include "Character/Abilities/A1CharAbilitySystemComponent.h"
#include "Character/Abilities/A1CharGameplayAbility.h"
#include "Character/Abilities/AttributeSets/A1CharacterAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGAS1Character::AGAS1Character(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAlwaysRelevant = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

UAbilitySystemComponent* AGAS1Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AGAS1Character::IsAlive() const
{
	return GetHealth() > 0.f;
}

int32 AGAS1Character::GetAbilityLevel(GAS1AbilityID AbilityID) const
{
	return 1;
}

void AGAS1Character::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
		return;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void AGAS1Character::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsEreMoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
		PlayAnimMontage(DeathMontage);
	else
	{
		FinishDying();
	}
}

void AGAS1Character::FinishDying()
{
	Destroy();
}

float AGAS1Character::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
		return AttributeSet->GetLevel();
	return 0.f;
}

float AGAS1Character::GetHealth() const
{
	if (AttributeSet.IsValid())
		return AttributeSet->GetHealth();
	return 0.f;
}

float AGAS1Character::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
		return AttributeSet->GetMaxHealth();
	return 0.f;
}

float AGAS1Character::GetMana() const
{
	if (AttributeSet.IsValid())
		return AttributeSet->GetMaxMana();
	return 0.f;
}

float AGAS1Character::GetMaxMana() const
{
	if (AttributeSet.IsValid())
		return AttributeSet->GetMana();
	return 0.f;
}

// Called when the game starts or when spawned
void AGAS1Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGAS1Character::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
		return;

	UE_LOG(LogTemp, Warning, TEXT("///// AddCharacterAbilities /////"));

	for (TSubclassOf<UA1CharGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AGAS1Character::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
		return;

	if (!DefaultAttributes)
		UE_LOG(LogTemp, Warning, TEXT("%s() Missing DefaultAttributes for %s. Please fill int character's bp"), *FString(__FUNCTION__), *GetName());

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());

}

void AGAS1Character::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AGAS1Character::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
		AttributeSet->SetHealth(Health);
}

void AGAS1Character::SetMana(float Mana)
{
	if (AttributeSet.IsValid())
		AttributeSet->SetMana(Mana);
}

// Called every frame
void AGAS1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGAS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



