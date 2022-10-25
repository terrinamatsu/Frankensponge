// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/InputOutput2_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../../Public/Effects/InputOutput2.h"	

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInputOutput2CtorTest, "InputOutput2.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInputOutput2CtorTest::RunTest(FString const& Parameters)
{
	// UE_LOG(Error) to fail the test
	// UE_LOG(Warning) to pass the test with a warning
	
	// Spawn InputOutput2 Actor
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AInputOutput2* InOut = World->SpawnActor<AInputOutput2>();

	// Default Variable Values
	TestEqual("InputOutput2 should have ParticleMaxFill default value of 20.0f", InOut->ParticleMaxFill, 20.0f);

	// Default Components Added to Actor
	TestTrue("InputOutput2 should create default UNiagaraComponent for TestNiagaraSystem", IsValid(InOut->TestNiagaraSystem));
	TestTrue("InputOutput2 should create default UBoxComponent for BoundingBox", IsValid(InOut->BoundingBox));

	// Default Attachments Set
	TestTrue("InputOutput2 should have TestNiagaraSystem as Root Component", InOut->TestNiagaraSystem == InOut->GetRootComponent());
	TestTrue("InputOutput2 should have BoundingBox attached to TestNiagaraSystem", InOut->BoundingBox->GetAttachParent() == InOut->TestNiagaraSystem);

	// Default Visibility, Meshes, & Materials
	TestFalse("InputOutput2 should have default TestNiagaraSystem visibility as True", InOut->TestNiagaraSystem->bHiddenInGame);
	TestTrue("InputOutput2 should have default BoundingBox visibility as False", InOut->BoundingBox->bHiddenInGame);

	// Default Collisions


	return true;
}

// Test Auto Get Player Pawn