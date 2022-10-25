// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/WaterBallProj_Tests.h"


#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../../Public/Effects/WaterBallProj.h"	

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaterBallProjCtorTest, "WaterBallProj.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWaterBallProjCtorTest::RunTest(FString const& Parameters)
{
	// UE_LOG(Error) to fail the test
	// UE_LOG(Warning) to pass the test with a warning

	// Spawn InputOutput2 Actor
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AWaterBallProj* WaterBallProj = World->SpawnActor<AWaterBallProj>();

	// Default Variable Values
	//TestEqual("WaterBallProj should have ParticleMaxFill default value of 20.0f", WaterBallProj->ParticleMaxFill, 20.0f);
	
	// Default Components Added to Actor
	TestTrue("WaterBallProj should create default UStaticMeshComponent for WaterBallModel", IsValid(WaterBallProj->WaterBallModel));
	TestTrue("WaterBallProj should create default USphereComponent for BoundingBox", IsValid(WaterBallProj->BoundSphere));
	
	// Default Attachments Set
	TestTrue("WaterBallProj should have WaterBallModel as Root Component", WaterBallProj->WaterBallModel == WaterBallProj->GetRootComponent());
	TestTrue("WaterBallProj should have BoundSphere attached to WaterBallModel", WaterBallProj->BoundSphere->GetAttachParent() == WaterBallProj->WaterBallModel);
	
	// Default Visibility, Meshes, & Materials
	TestFalse("WaterBallProj should have default WaterBallModel visibility as True", WaterBallProj->WaterBallModel->bHiddenInGame);
	TestTrue("WaterBallProj should have default BoundSphere visibility as False", WaterBallProj->BoundSphere->bHiddenInGame);

	// Default Collisions


	return true;
}