// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSponge.h"

// Sets default values
ACharacterSponge::ACharacterSponge()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterSponge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterSponge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterSponge::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

