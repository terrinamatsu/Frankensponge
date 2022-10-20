// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "../Effects/WaterBallProj.h"

#include "PawnPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbsorbPressDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbsorbHoldDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbsorbReleaseDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleasePressDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseHoldDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseReleaseDelegate);


UCLASS()
class FRANKENSPONGE_API APawnPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnPlayer();

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
		void ThrowWaterPress();
	UFUNCTION()
		void ThrowWaterRelease();

	UPROPERTY(BlueprintAssignable)
		FOnAbsorbPressDelegate OnAbsorbPress;
	UPROPERTY(BlueprintAssignable)
		FOnAbsorbHoldDelegate OnAbsorbHold;
	UPROPERTY(BlueprintAssignable)
		FOnAbsorbReleaseDelegate OnAbsorbRelease;

	UPROPERTY(BlueprintAssignable)
		FOnReleasePressDelegate OnReleasePress;
	UPROPERTY(BlueprintAssignable)
		FOnReleaseHoldDelegate OnReleaseHold;
	UPROPERTY(BlueprintAssignable)
		FOnReleaseReleaseDelegate OnReleaseRelease;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	// Model
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Model;


	UMaterialInterface* MaterialSponge;
	UMaterialInstanceDynamic* SpongeMaterial;

	bool bIsCrouching = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsAbsorbing = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsReleasing = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AmountAbsorbed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PercentAbsorbed = 0.0f;

	UFUNCTION()
		void CalcAbsorbtion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsHoldingThrow;

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

	UFUNCTION(BlueprintCallable)
		bool GetIsAbsorbing();
	UFUNCTION()
		bool GetIsReleasing();

	// Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* BoundingBox;

	UFUNCTION(BlueprintCallable)
		void Absorb(float _input);
	UFUNCTION(BlueprintCallable)
		void Release(float _output);
	// Absorb / Release Calculations, returns how much can be absorbed/released this time step
	UFUNCTION(BlueprintCallable)
		float CalcAbsorb(const float _dt);
	UFUNCTION(BlueprintCallable)
		float CalcRelease(const float _dt);
};
