// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceExport.h"

#include "../Characters/CharPlayer.h"

#include "InputOutput2.generated.h"


UCLASS()
class FRANKENSPONGE_API AInputOutput2 : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInputOutput2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float SpongeRelease(const TArray<FBasicParticleData>& Data, float _ReleaseAmount);
	float SpongeAbsorb(const TArray<FBasicParticleData>& Data, float _AbsorbAmount);

	// Last Game Seconds on particle data function call
	float AbsorbTimerSecs;
	// Last Delta Time on particle data function call
	float AbsorbPreTime;

	TArray<uint32> FindPlayerBoundedParticles(const TArray<FBasicParticleData>& Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraComponent* TestNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoundingBox;

	ACharPlayer* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ParticleMaxFill = 20.0f;

	///UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		///void ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset);
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;
};
