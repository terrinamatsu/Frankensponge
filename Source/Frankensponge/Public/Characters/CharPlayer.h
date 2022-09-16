// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CharPlayer.generated.h"

UCLASS()
class FRANKENSPONGE_API ACharPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement Functions
	UFUNCTION()
		void MoveLeftRight(float AxisValue);
	UFUNCTION()
		void MoveCrouch();
	UFUNCTION()
		void MoveReleaseCrouch();

	// Action Functions
	UFUNCTION()
		void MoveJump();
	UFUNCTION()
		void ActionAbsorb();
	UFUNCTION()
		void ActionReleaseAbsorb();
	UFUNCTION()
		void ActionRelease();
	UFUNCTION()
		void ActionReleaseRelease();
	UFUNCTION()
		void ThrowWater();

	UFUNCTION()
		void Absorb(float _dt);
	UFUNCTION()
		void Release(float _dt);

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	// Model
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Model;

	bool bIsCrouching = false;
	bool bIsAbsorbing = false;
	bool bIsReleasing = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AmountAbsorbed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Model & Absorbtion Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinScale = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxScale = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAbsorb = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AbsorbRate = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ReleaseRate = 70.0f;
};
