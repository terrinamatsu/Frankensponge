// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/CharPlayer_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../../Public/Characters/CharPlayer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharPlayerCtorTest, "CharPlayer.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCharPlayerCtorTest::RunTest(FString const& Parameters)
{
	// UE_LOG(Error) to fail the test
	// UE_LOG(Warning) to pass the test with a warning

	// Spawn Player Character
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ACharPlayer* Player = World->SpawnActor<ACharPlayer>();

	// Default Variable Values
	TestEqual("CharPlayer should have MinScale default value of 5.0f", Player->MinScale, 5.0f);
	TestEqual("CharPlayer should have MaxScale default value of 15.0f", Player->MaxScale, 15.0f);
	TestEqual("CharPlayer should have MaxAbsorb default value of 5000.0f", Player->MaxAbsorb, 5000.0f);
	TestEqual("CharPlayer should have AbsorbRate default value of 500.0f", Player->AbsorbRate, 500.0f);
	TestEqual("CharPlayer should have ReleaseRate default value of 700.0f", Player->ReleaseRate, 700.0f);

	// Default Components Added to Actor
	TestTrue("CharPlayer should create default UCameraComponent for Camera", IsValid(Player->Camera));
	TestTrue("CharPlayer should create default UStaticMeshComponent for Model", IsValid(Player->Model));
	TestTrue("CharPlayer should create default UBoxComponent for BoundingBox", IsValid(Player->BoundingBox));

	// Default Attachments Set
	TestTrue("CharPlayer should have Camera attached to Root Component", Player->Camera->GetAttachParent() == Player->GetRootComponent());
	TestTrue("CharPlayer should have Model attached to Root Component", Player->Model->GetAttachParent() == Player->GetRootComponent());
	TestTrue("CharPlayer should have BoundingBox attached to Model", Player->BoundingBox->GetAttachParent() == Player->Model);

	// Default Visibility, Meshes, & Materials
	TestFalse("CharPlayer should have default Model visibility as True", Player->Model->bHiddenInGame);
	TestTrue("CharPlayer should have default BoundingBox visibility as False", Player->BoundingBox->bHiddenInGame);

	// Default Collisions
	
	// Default "GetIsReleasing/GetIsAbsorbing" Values
	TestEqual("CharPlayer should not be absorbing by default (GetIsAbsorbing() == false)", Player->GetIsAbsorbing(), false);
	TestEqual("CharPlayer should not be releasing by default (GetIsReleasing() == false)", Player->GetIsReleasing(), false);
	TestEqual("CharPlayer should have PercentAbsorbed default value of 0.0f", Player->GetPercentAbsorbed(), 0.0f);
	
	return true;
}

// TEST:: Absorb & Release Functions
// TEST:: Calc Absorb & Release Functions