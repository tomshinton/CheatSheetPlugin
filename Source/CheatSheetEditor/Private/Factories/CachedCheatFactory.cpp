// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetEditor/Public/Factories/CachedCheatFactory.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatAsset.h>

UCachedCheatFactory::UCachedCheatFactory()
{
	SupportedClass = UCachedCheatAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCachedCheatFactory::ShouldShowInNewMenu() const
{
	return true;
}

UObject* UCachedCheatFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (UCachedCheatAsset* NewCheat = NewObject<UCachedCheatAsset>(InParent, InClass, InName, Flags))
	{
		return NewCheat;
	}

	return nullptr;
}
