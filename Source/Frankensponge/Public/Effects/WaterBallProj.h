// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/SphereComponent.h"

#include "WaterBallProj.generated.h"

UCLASS()
class FRANKENSPONGE_API AWaterBallProj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterBallProj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* BoundSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* WaterBallModel;

	UFUNCTION() // Boundary Overlap Event
		void OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
