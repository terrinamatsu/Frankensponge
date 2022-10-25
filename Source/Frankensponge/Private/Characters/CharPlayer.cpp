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

	/* Setup Components */
	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	// Model
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrankenSpongeMesh(TEXT("StaticMesh'/Game/Models/Frankensponge/FrankenTest.FrankenTest'"));
	Model->SetStaticMesh(FrankenSpongeMesh.Object);
	Model->SetupAttachment(RootComponent);

	Model->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
	Model->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	Model->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	// Bounding Box
	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingBox"));
	BoundingBox->SetupAttachment(Model);
	BoundingBox->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	BoundingBox->SetRelativeScale3D(FVector(0.155f, 0.0825f, 0.155f));

	// Sponge Model Dynamic Material
	//SpongeMaterial = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("SpongeMaterial"));
	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> FrankenSpongeMaterial(TEXT("Material'/Game/Models/Frankensponge/M_Frankensponge.M_Frankensponge'"));
	if (FrankenSpongeMaterial.Succeeded())
	{
		MaterialSponge = FrankenSpongeMaterial.Object;
	}
	SpongeMaterial = UMaterialInstanceDynamic::Create(MaterialSponge, Model);
	Model->SetMaterial(0, SpongeMaterial);*/

	// Move Camera back
	Camera->SetRelativeLocation(FVector(- 300.0f, 0.0f, 0.0f));

	// Setup Crouching Controls
	GetCharacterMovement()->CrouchedHalfHeight = 88.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
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

	// Sponge Scaling based on current water absorbtion percent
	// maybe not linear scaling, scale based on volume?
	float CurrentScalar = ((MaxScale - MinScale) * (AmountAbsorbed / MaxAbsorb)) + MinScale;
	FVector CurrentModelScale = FVector(CurrentScalar, CurrentScalar, CurrentScalar);

	// Half height if crouching
	if (bIsCrouching)
	{
		CurrentModelScale.Z /= 2;
	}

	Model->SetRelativeScale3D(CurrentModelScale);

	// Release into void
	if (TickAmountReleased < (ReleaseRate * DeltaTime) && bIsReleasing)
	{
		Release((ReleaseRate * DeltaTime) - TickAmountReleased);
	}

	TickAmountReleased = 0.0f;


	// Trigger on Absorb & Release events
	if (bIsAbsorbing)
	{
		//Absorb(DeltaTime);
		//OnAbsorbHold.Broadcast(DeltaTime);
	}
	if (bIsReleasing)
	{
		//Release(DeltaTime);
		//OnReleaseHold.Broadcast(DeltaTime);
	}

	// Adjust Sponge Material Wetness based on current absorbtion
	//SpongeMaterial->SetScalarParameterValue("Wetness", PercentAbsorbed);

	// Adjust Movement based on current absorbtion
	GetCharacterMovement()->GroundFriction = (4.0f * (1.0f - PercentAbsorbed)) + 0.1f;
	GetCharacterMovement()->JumpZVelocity = (320.0f * (1.0f - PercentAbsorbed)) + 200.0f;
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
	PlayerInputComponent->BindAction("ThrowWater", EInputEvent::IE_Pressed, this, &ACharPlayer::ThrowWaterPress);
	PlayerInputComponent->BindAction("ThrowWater", EInputEvent::IE_Released, this, &ACharPlayer::ThrowWaterRelease);

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
	Crouch();
	bIsCrouching = true;
}
void ACharPlayer::MoveReleaseCrouch()
{
	UnCrouch();
	bIsCrouching = false;
}

void ACharPlayer::ActionAbsorb()
{
	bIsAbsorbing = true;
	//OnAbsorbPress.Broadcast();
}
void ACharPlayer::ActionReleaseAbsorb()
{
	bIsAbsorbing = false;
	//OnAbsorbRelease.Broadcast();
}
void ACharPlayer::Absorb(float _input)
{
	AmountAbsorbed += _input; // * AbsorbRate;
	CalcAbsorbtion();
}


void ACharPlayer::ActionRelease()
{
	bIsReleasing = true;
	//OnReleasePress.Broadcast();
}
void ACharPlayer::ActionReleaseRelease()
{
	bIsReleasing = false;
	//OnReleaseRelease.Broadcast();
}
void ACharPlayer::Release(float _output)
{
	AmountAbsorbed -= _output; // * ReleaseRate;
	CalcAbsorbtion();

	TickAmountReleased += _output;
}

void ACharPlayer::ThrowWaterPress()
{
	bIsHoldingThrow = true;
}

void ACharPlayer::ThrowWaterRelease()
{
	bIsHoldingThrow = false;

	// Throw waterball with size based on ThrowWater button hold length
	FVector ProjLoc = GetActorLocation() + FVector(0.0f, 0.0f, 120.0f);
	
	FVector MoveAxis = FVector(GetInputAxisValue("MoveLeftRight"), 0.0f, GetInputAxisValue("MoveUpDown"));
	FRotator ProjRot = MoveAxis.Rotation();

	AWaterBallProj* ThrownWaterBallProj = GetWorld()->SpawnActor<AWaterBallProj>(ProjLoc, ProjRot);

	//ThrownWaterBallProj->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	//ThrownWaterBallProj.
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

float ACharPlayer::CalcAbsorb(const float _dt)
{
	float TotalAbsorbtion = AbsorbRate * _dt;

	// Clamp absorbtion to absorb rate, one second of absorbtion. 
	if (TotalAbsorbtion > AbsorbRate)
	{
		TotalAbsorbtion = AbsorbRate;
	}

	// Clamp Absorbtion to MaxAbsorb of the sponge
	if ((TotalAbsorbtion + AmountAbsorbed) > MaxAbsorb)
	{
		TotalAbsorbtion = MaxAbsorb - AmountAbsorbed;
	}

	return TotalAbsorbtion;
}

float ACharPlayer::CalcRelease(const float _dt)
{
	float TotalRelease = ReleaseRate * _dt;

	// Clamp release to release rate, one second of release. 
	if (TotalRelease > ReleaseRate)
	{
		TotalRelease = ReleaseRate;
	}

	// Clamp amount to release to the amount left in the sponge
	if ((AmountAbsorbed - TotalRelease) < 0)
	{
		TotalRelease = AmountAbsorbed;
	}

	return TotalRelease;
}

float ACharPlayer::GetPercentAbsorbed()
{
	return PercentAbsorbed;
}