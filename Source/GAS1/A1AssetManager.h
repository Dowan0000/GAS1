// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "A1AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GAS1_API UA1AssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	virtual void StartInitialLoading() override;
};
