// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharPlayer.h"

// Sets default values
ACharPlayer::ACharPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	GetCharacterMovement()->AirControl = 0.5f;

	// Setup Components
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrankenSpongeMesh(TEXT("StaticMesh'/Game/Models/Frankensponge/FrankenTest.FrankenTest'"));
	Model->SetStaticMesh(FrankenSpongeMesh.Object);
	Model->SetupAttachment(RootComponent);

	Model->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
	Model->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	Model->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

// Called when the game starts or when spawned
void ACharPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// maybe not linear scaling, scale based on volume?
	float CurrentScalar = ((MaxScale - MinScale) * (AmountAbsorbed / MaxAbsorb)) + MinScale;
	FVector CurrentModelScale = FVector(CurrentScalar, CurrentScalar, CurrentScalar);

	if (bIsCrouching)
	{
		CurrentModelScale.Z /= 2;
	}

	Model->SetRelativeScale3D(CurrentModelScale);

	if (bIsAbsorbing)
	{
		//Absorb(DeltaTime);
		OnAbsorbHold.Broadcast(DeltaTime);
	}
	if (bIsReleasing)
	{
		//Release(DeltaTime);
		OnReleaseHold.Broadcast(DeltaTime);
	}
}

// Called to bind functionality to input
void ACharPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Bind Actions */
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharPlayer::MoveJump);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ACharPlayer::MoveCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &ACharPlayer::MoveReleaseCrouch);
	PlayerInputComponent->BindAction("Absorb", EInputEvent::IE_Pressed, this, &ACharPlayer::ActionAbsorb);
	PlayerInputComponent->BindAction("Absorb", EInputEvent::IE_Released, this, &ACharPlayer::ActionReleaseAbsorb);
	PlayerInputComponent->BindAction("Release", EInputEvent::IE_Pressed, this, &ACharPlayer::ActionRelease);
	PlayerInputComponent->BindAction("Release", EInputEvent::IE_Released, this, &ACharPlayer::ActionReleaseRelease);
	PlayerInputComponent->BindAction("ThrowWater", EInputEvent::IE_Pressed, this, &ACharPlayer::ThrowWater);

	/* Bind Axis */
	PlayerInputComponent->BindAxis("MoveLeftRight", this, &ACharPlayer::MoveLeftRight);
}

void ACharPlayer::MoveLeftRight(float AxisValue)
{
	// Find out which way is "right" move the player in that direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}

void ACharPlayer::MoveJump()
{
	Jump();
}

void ACharPlayer::MoveCrouch()
{
	bIsCrouching = true;
}
void ACharPlayer::MoveReleaseCrouch()
{
	bIsCrouching = false;
}

void ACharPlayer::ActionAbsorb()
{
	bIsAbsorbing = true;
	OnAbsorbPress.Broadcast();
}
void ACharPlayer::ActionReleaseAbsorb()
{
	bIsAbsorbing = false;
	OnAbsorbRelease.Broadcast();
}
void ACharPlayer::Absorb(float _input)
{
	AmountAbsorbed += _input; // * AbsorbRate;
	CalcAbsorbtion();
}


void ACharPlayer::ActionRelease()
{
	bIsReleasing = true;
	OnReleasePress.Broadcast();
}
void ACharPlayer::ActionReleaseRelease()
{
	bIsReleasing = false;
	OnReleaseRelease.Broadcast();
}
void ACharPlayer::Release(float _output)
{
	AmountAbsorbed -= _output; // * ReleaseRate;
	CalcAbsorbtion();
}

void ACharPlayer::ThrowWater()
{
	Jump();
}

void ACharPlayer::CalcAbsorbtion()
{
	if (AmountAbsorbed < 0.0f)
	{
		AmountAbsorbed = 0.0f;
	}
	else if (AmountAbsorbed > MaxAbsorb)
	{
		AmountAbsorbed = MaxAbsorb;
	}

	PercentAbsorbed = (AmountAbsorbed / MaxAbsorb);
}

bool ACharPlayer::GetIsAbsorbing()
{
	return bIsAbsorbing;
}

bool ACharPlayer::GetIsReleasing()
{
	return bIsReleasing;
}