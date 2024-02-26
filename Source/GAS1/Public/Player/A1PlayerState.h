// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Character/Abilities/AttributeSets/A1CharacterAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "A1PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAS1_API AA1PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AA1PlayerState();

	virtual void BeginPlay() override;

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UA1CharacterAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|Attributes")
		float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|Attributes")
		float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "A1PlayerState|Attributes")
		int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
		class UA1CharAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
		class UA1CharacterAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
