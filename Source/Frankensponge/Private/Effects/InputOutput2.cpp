// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/InputOutput2.h"

// Sets default values
AInputOutput2::AInputOutput2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Components
	TestNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TestNiagaraComponent"));
	SetRootComponent(TestNiagaraSystem);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> InputOutputSystem(TEXT("NiagaraSystem'/Game/Effects/P_InputOutputTest.P_InputOutputTest'"));
	TestNiagaraSystem->SetAsset(InputOutputSystem.Object);


	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingBox"));
	BoundingBox->SetupAttachment(TestNiagaraSystem);
	BoundingBox->SetRelativeScale3D(FVector(1.0f, 1.38f, 1.36f));

	// Get reference to player CharPlayer pawn.
	APawn* CurrentPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACharPlayer* CurrentCharPlayer = dynamic_cast<ACharPlayer*>(CurrentPlayerPawn);
	if (CurrentCharPlayer)
	{
		Player = CurrentCharPlayer;
	}

	// Setup Timer for Particle Data Receive Calls
	AbsorbTimerSecs = UGameplayStatics::GetTimeSeconds(GetWorld());
	AbsorbPreTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}

// Called when the game starts or when spawned
void AInputOutput2::BeginPlay()
{
	Super::BeginPlay();
	
	// Get reference to player CharPlayer pawn.
	APawn* CurrentPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACharPlayer* CurrentCharPlayer = dynamic_cast<ACharPlayer*>(CurrentPlayerPawn);
	if (CurrentCharPlayer)
	{
		Player = CurrentCharPlayer;
	}

	TestNiagaraSystem->SetVariableObject("PosExport", this);
}

// Called every frame
void AInputOutput2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputOutput2::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	// Calculate Delta Time
	float CurTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	float CurDT = 0.0f;

	float AbsorbTimer = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	/*
	if (CurTime != AbsorbTimerSecs)
	{
		AbsorbTimerSecs = CurTime;
		CurDT = 1 - AbsorbPreTime + AbsorbTimer;
	}
	else
	{
		CurDT = AbsorbTimer - AbsorbPreTime;
	}

	AbsorbPreTime = AbsorbTimer;

	GLog->Log(FString::Printf(TEXT("%f;; %f"), AbsorbTimer, CurDT)); */


	//\ never mind it's probably just better to use raw world delta seconds.
	CurDT = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	// Calculate Player Absorbing & Releasing (Inputting new data to Niagara System)
	if (Player)
	{
		if (Player->GetIsReleasing() && BoundingBox->IsOverlappingComponent(Player->BoundingBox))
		{
			Player->Release(SpongeRelease(Data, Player->CalcRelease(CurDT)));
		}
		else if (Player->GetIsAbsorbing() && BoundingBox->IsOverlappingComponent(Player->BoundingBox))
		{
			Player->Absorb(SpongeAbsorb(Data, Player->CalcAbsorb(CurDT)));
		}
	}
}

float AInputOutput2::SpongeRelease(const TArray<FBasicParticleData>& Data, float _ReleaseAmount)
{
	float TotalReleased = 0.0f;
	TArray<FVector2D> ParticleSizes;

	TArray<uint32> OverlapParticleIndices = FindPlayerBoundedParticles(Data);

	/*//for (auto& Particle : Data)
	//{
	//	// Calculate if particle is within bounding box of the player sponge. 
	//	FTransform PlayerBoxTransform = Player->BoundingBox->GetComponentTransform();
	//	FVector PlayerBoxExtent = Player->BoundingBox->GetComponentScale() * 30.0f;
	//	
	//	FVector HitLocation; FVector HitNormal; FName BoneName; FHitResult OutHit;

	//	//if (UKismetMathLibrary::IsPointInBoxWithTransform(Particle.Position, PlayerBoxTransform, PlayerBoxExtent))
	//	if (Player->BoundingBox->K2_SphereOverlapComponent(Particle.Position, Particle.Size, false, false, false, HitLocation, HitNormal, BoneName, OutHit))
	//	{
	//		ParticleSizes.Add(FVector2D(Particle.Size));
	//	}
	//	else
	//	{
	//		float NewSize = UKismetMathLibrary::FClamp(((AbsorbTimer * 100.0f) + Particle.Size), 0.0f, ParticleMaxFill);
	//		ParticleSizes.Add(FVector2D(NewSize));

	//		TotalReleased += NewSize - Particle.Size;
	//	}
	//} */

	// Set new particle size array data
	for (auto& Particle : Data)
	{
		ParticleSizes.Add(FVector2D(Particle.Size));
	}

	// Go through overlap indices to find total remaining particle volume being taken away from
	float TotalParticleVolume = 0.0f;
	for (auto& i : OverlapParticleIndices)
	{
		TotalParticleVolume += ParticleMaxFill - ParticleSizes[i].X;
	}

	// Calculate percentage of total volume that is being filled this tick
	float FillPercent = _ReleaseAmount / TotalParticleVolume;

	// Go through overlap indices to add new amount to size
	for (auto i : OverlapParticleIndices)
	{
		// Calculate percent of remaining particle size to be filled this tick
		float ToFill= (ParticleMaxFill - Data[i].Size) * FillPercent;

		float NewSize = UKismetMathLibrary::FClamp((Data[i].Size + ToFill), 0.0f, ParticleMaxFill);
		ParticleSizes[i] = FVector2D(NewSize);

		TotalReleased += NewSize - Data[i].Size;
	}

	// Import new particle size data to Niagara System
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector2D(TestNiagaraSystem, "Import", ParticleSizes);

	return TotalReleased;
}

float AInputOutput2::SpongeAbsorb(const TArray<FBasicParticleData>& Data, float _AbsorbAmount)
{
	float TotalAbsorbed = 0.0f;
	TArray<FVector2D> ParticleSizes;

	TArray<uint32> OverlapParticleIndices = FindPlayerBoundedParticles(Data);

	/*//for (auto i = 0; i < Data.Num(); ++i)
	//{		
	//	// Check if index is the next overlap index, and is not the last (null) index
	//	if(i == OverlapParticleIndices[IndicesIndex] && IndicesIndex != OverlapParticleIndices.Num())
	//	{
	//		IndicesIndex++;
	//		ParticleSizes.Add(FVector2D(Data[i].Size));
	//	}
	//	else
	//	{
	//		// Recalculate PerParticleAbsorb
	//		ParParticleAbsorb = (_AbsorbAmount - TotalAbsorbed) / (OverlapParticleIndices.Num() - IndicesIndex - 1);

	//		float NewSize = UKismetMathLibrary::FClamp((Data[i].Size - ParParticleAbsorb), 0.0f, 50.0f);
	//		ParticleSizes.Add(FVector2D(NewSize));

	//		TotalAbsorbed += Data[i].Size - NewSize;
	//	}
	//}*/

	// Set new particle size array data
	for (auto& Particle : Data)
	{
		ParticleSizes.Add(FVector2D(Particle.Size));
	}

	// Go through overlap indices to find total particle volume being written into
	float TotalParticleVolume = 0.0f;
	for (auto& i : OverlapParticleIndices)
	{
		TotalParticleVolume += ParticleMaxFill - ParticleSizes[i].X;
	}
	
	// Calculate percentage of total volume that is being absorbed from this tick
	float FillPercent = TotalParticleVolume;

	// Go through overlap indices to add new amount to size
	for (auto i : OverlapParticleIndices)
	{
		// Calculate percent of remaining particle size to be filled this tick
		float ToTake = (Data[i].Size) * FillPercent;

		float NewSize = UKismetMathLibrary::FClamp((Data[i].Size - ToTake), 0.0f, ParticleMaxFill);
		ParticleSizes[i] = FVector2D(NewSize);

		TotalAbsorbed += Data[i].Size - NewSize;
	}

	// Import new particle size data to Niagara System
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector2D(TestNiagaraSystem, "Import", ParticleSizes);

	//GLog->Log(FString::Printf(TEXT("TotalParticleVolume: %f, FillPercent: %f, OverlapParticleIndicesNum: %d, TotalAbsorbed: %f"), TotalParticleVolume, FillPercent, OverlapParticleIndices.Num(), TotalAbsorbed));

	return TotalAbsorbed;
}

TArray<uint32> AInputOutput2::FindPlayerBoundedParticles(const TArray<FBasicParticleData>& Data)
{
	// Calculate if particle is within bounding box of the player sponge.
	TArray<uint32> OverlapParticleIndices;

	//// Bounding Box Collision Version
	//FTransform PlayerBoxTransform = Player->BoundingBox->GetComponentTransform();
	//FVector PlayerBoxExtent = Player->BoundingBox->GetComponentScale() * 30.0f;
	//if (UKismetMathLibrary::IsPointInBoxWithTransform(Particle.Position, PlayerBoxTransform, PlayerBoxExtent))

	FVector HitLocation; FVector HitNormal; FName BoneName; FHitResult OutHit;

	for (auto i = 0; i < Data.Num(); ++i)
	{
		if (Player->BoundingBox->K2_SphereOverlapComponent(Data[i].Position, Data[i].Size, false, false, false, HitLocation, HitNormal, BoneName, OutHit))
		{
			OverlapParticleIndices.Add(i);
		}
	}

	return OverlapParticleIndices;
}