// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/A1PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/PlayerAIController.h"
#include "Player/A1PlayerState.h"
#include "Player/A1PlayerController.h"
#include "Character/Abilities/A1CharAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../GAS1.h"

AA1PlayerCharacter::AA1PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 70.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = APlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void AA1PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AA1PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AA1PlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Lookup", this, &AA1PlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookupRate", this, &AA1PlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("Turn", this, &AA1PlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AA1PlayerCharacter::TurnRate);

	BindASCInput();
}

void AA1PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AA1PlayerState* PS = GetPlayerState<AA1PlayerState>();
	if (PS)
	{
		InitializStartingValues(PS);

		AddStartupEffects();
		AddCharacterAbilities();
	}
}

USpringArmComponent* AA1PlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent* AA1PlayerCharacter::GetFollowCamera()
{
	return FollowCamera;
}

float AA1PlayerCharacter::GetStartingCameraBoomArmLength()
{
	return StartingCameraBoomArmLength;
}

FVector AA1PlayerCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

void AA1PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartingCameraBoomArmLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();

}

void AA1PlayerCharacter::LookUp(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value);
	}
}

void AA1PlayerCharacter::LookUpRate(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void AA1PlayerCharacter::Turn(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void AA1PlayerCharacter::TurnRate(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void AA1PlayerCharacter::MoveForward(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AA1PlayerCharacter::MoveRight(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AA1PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AA1PlayerState* PS = GetPlayerState<AA1PlayerState>();
	if (PS)
	{
		InitializStartingValues(PS);

		BindASCInput();
	}
}

void AA1PlayerCharacter::InitializStartingValues(AA1PlayerState* PS)
{
	AbilitySystemComponent = Cast<UA1CharAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSet = PS->GetAttributeSetBase();

	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	InitializeAttributes();
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
}

void AA1PlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GAS1"), FName("GAS1AbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(GAS1AbilityID::Confirm), static_cast<int32>(GAS1AbilityID::Cancel)));
		
		ASCInputBound = true;
	}
}
