// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Editor/UnrealEd/Classes/Factories/Factory.h>

#include "CachedCheatBuilderFactory.generated.h"

UCLASS(hideCategories=Object)
class UCachedCheatBuilderFactory : public UFactory
{
	GENERATED_BODY()

public:

	UCachedCheatBuilderFactory();

	//UFactory
	bool ShouldShowInNewMenu() const override;
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~UFactory
};
