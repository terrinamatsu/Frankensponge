// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PawnMovementPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FRANKENSPONGE_API UPawnMovementPlayer : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//void AddInputVector(FVector WorldVector, bool bForce) override;
};
