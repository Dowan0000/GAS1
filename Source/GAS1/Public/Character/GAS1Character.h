// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "../../GAS1.h"
#include "GameplayTagContainer.h"
#include "GAS1Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGAS1Character*, character);

UCLASS()
class GAS1_API AGAS1Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGAS1Character(const class FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "GAS1Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	virtual int32 GetAbilityLevel(GAS1AbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "GAS1Character")
	float GetMaxMana() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UA1CharAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UA1CharacterAttributeSet> AttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS1Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UA1CharGameplayAbility>> CharacterAbilities;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);
	
	virtual void SetMana(float Mana);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
