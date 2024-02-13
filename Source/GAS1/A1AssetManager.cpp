// Fill out your copyright notice in the Description page of Project Settings.


#include "A1AssetManager.h"
#include "AbilitySystemGlobals.h"

void UA1AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	
	UE_LOG(LogTemp, Warning, TEXT("Hello GAS"));
}
