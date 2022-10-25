// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/WaterSpout_Tests.h"


#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../../Public/Effects/WaterSpout.h"	

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaterSpoutCtorTest, "WaterSpout.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWaterSpoutCtorTest::RunTest(FString const& Parameters)
{
	// UE_LOG(Error) to fail the test
	// UE_LOG(Warning) to pass the test with a warning

	// Spawn InputOutput2 Actor
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AWaterSpout* WaterSpout = World->SpawnActor<AWaterSpout>();

	// Default Variable Values
	TestEqual("WaterSpout should have bIsBeingAbsorbed default value of false", WaterSpout->bIsBeingAbsorbed, false);
	
	// Default Components Added to Actor
	TestTrue("WaterSpout should create default UNiagaraComponent for TestNiagaraSystem", IsValid(WaterSpout->TestNiagaraSystem));
	TestTrue("WaterSpout should create default UBoxComponent for BoundingBox", IsValid(WaterSpout->BoundingBox));
	
	// Default Attachments Set
	TestTrue("WaterSpout should have TestNiagaraSystem as Root Component", WaterSpout->TestNiagaraSystem == WaterSpout->GetRootComponent());
	TestTrue("WaterSpout should have BoundingBox attached to TestNiagaraSystem", WaterSpout->BoundingBox->GetAttachParent() == WaterSpout->TestNiagaraSystem);
	
	// Default Visibility, Meshes, & Materials
	TestFalse("WaterSpout should have default TestNiagaraSystem visibility as True", WaterSpout->TestNiagaraSystem->bHiddenInGame);
	TestTrue("WaterSpout should have default BoundingBox visibility as False", WaterSpout->BoundingBox->bHiddenInGame);
	
	// Default Collisions


	return true;
}

// Test Auto Get Player Pawn