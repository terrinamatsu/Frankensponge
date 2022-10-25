// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WaterBallProj.h"

// Sets default values
AWaterBallProj::AWaterBallProj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Water Ball Model
	WaterBallModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterBall"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		WaterBallModel->SetStaticMesh(SphereMesh.Object);
	}
	this->SetRootComponent(WaterBallModel);
	WaterBallModel->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));

	// Setup Bound Sphere Component
	BoundSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BoundSphere"));
	BoundSphere->OnComponentBeginOverlap.AddDynamic(this, &AWaterBallProj::OnBoundBeginOverlap);
	BoundSphere->SetupAttachment(WaterBallModel);

	// Set Physics
	WaterBallModel->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWaterBallProj::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterBallProj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterBallProj::OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//this->Destroy();
}
