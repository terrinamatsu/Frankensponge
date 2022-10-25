// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WaterSpout.h"

// Sets default values
AWaterSpout::AWaterSpout()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Components
	TestNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TestNiagaraComponent"));
	SetRootComponent(TestNiagaraSystem);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> InputOutputSystem(TEXT("NiagaraSystem'/Game/Effects/Final/P_WaterFountain.P_WaterFountain'"));
	if (InputOutputSystem.Succeeded())
	{
		TestNiagaraSystem->SetAsset(InputOutputSystem.Object);
	}

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingBox"));
	BoundingBox->SetupAttachment(TestNiagaraSystem);
	BoundingBox->SetRelativeScale3D(FVector(1.0f, 1.38f, 1.36f));

	BoundingBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterSpout::OnBoundBeginOverlap);
	BoundingBox->OnComponentEndOverlap.AddDynamic(this, &AWaterSpout::OnBoundEndOverlap);
}

// Called when the game starts or when spawned
void AWaterSpout::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterSpout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBeingAbsorbed && PlayerChar->GetIsAbsorbing())
	{
		PlayerChar->Absorb(PlayerChar->AbsorbRate * DeltaTime);
	}
}

void AWaterSpout::OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharPlayer* Player = dynamic_cast<ACharPlayer*>(OtherActor);

	if (Player)
	{
		PlayerChar = Player;
		bIsBeingAbsorbed = true;

		//GLog->Log(TEXT("OVERLAP WATERSPOUT"));
	}
}

void AWaterSpout::OnBoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharPlayer* Player = dynamic_cast<ACharPlayer*>(OtherActor);

	if (Player)
	{
		PlayerChar = nullptr;
		bIsBeingAbsorbed = false;

		//GLog->Log(TEXT("END OVERLAP WATERSPOUT"));
	}
}