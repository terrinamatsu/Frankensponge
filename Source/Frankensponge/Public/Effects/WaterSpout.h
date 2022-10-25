// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/BoxComponent.h"

#include "NiagaraComponent.h"

#include "../Characters/CharPlayer.h"

#include "WaterSpout.generated.h"

UCLASS()
class FRANKENSPONGE_API AWaterSpout : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterSpout();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraComponent* TestNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoundingBox;

	ACharPlayer* PlayerChar;
	bool bIsBeingAbsorbed = false;

	UFUNCTION() // Boundary Overlap Event
		void OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() // Boundary End Overlap Event
		void OnBoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
