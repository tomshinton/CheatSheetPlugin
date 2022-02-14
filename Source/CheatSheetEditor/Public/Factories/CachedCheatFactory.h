// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Editor/UnrealEd/Classes/Factories/Factory.h>

#include "CachedCheatFactory.generated.h"

UCLASS(hideCategories=Object)
class UCachedCheatFactory : public UFactory
{
	GENERATED_BODY()

public:

	UCachedCheatFactory();

	//UFactory
	bool ShouldShowInNewMenu() const override;
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~UFactory
};
