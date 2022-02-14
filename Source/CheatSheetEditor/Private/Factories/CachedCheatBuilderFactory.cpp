// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetEditor/Public/Factories/CachedCheatBuilderFactory.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatBuilder.h>
#include <Editor/UnrealEd/Public/Kismet2/KismetEditorUtilities.h>

UCachedCheatBuilderFactory::UCachedCheatBuilderFactory()
{
	SupportedClass = UCachedCheatBuilder::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCachedCheatBuilderFactory::ShouldShowInNewMenu() const
{
	return true;
}

UObject* UCachedCheatBuilderFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(UCachedCheatBuilder::StaticClass(), InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
