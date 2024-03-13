// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAS1Character.h"
#include "A1PlayerCharacter.generated.h"

/**
 * 
 */

class AA1PlayerState;


UCLASS()
class GAS1_API AA1PlayerCharacter : public AGAS1Character
{
	GENERATED_BODY()
	
public:
	AA1PlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	class USpringArmComponent* GetCameraBoom();

	class UCameraComponent* GetFollowCamera();

	UFUNCTION(BlueprintCallable, Category = "Camera")
		float GetStartingCameraBoomArmLength();

	UFUNCTION(BlueprintCallable, Category = "Camera")
		FVector GetStartingCameraBoomLocation();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Camera")
		float BaseTurnRate = 45.f;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
		float BaseLookUpRate = 45.f;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		float StartingCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		FVector StartingCameraBoomLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
		class UCameraComponent* FollowCamera;

protected:
	virtual void BeginPlay() override;

	void LookUp(float Value);

	void LookUpRate(float Value);

	void Turn(float Value);

	void TurnRate(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void OnRep_PlayerState() override;

	void InitializStartingValues(AA1PlayerState* PS);

	void BindASCInput();

};
