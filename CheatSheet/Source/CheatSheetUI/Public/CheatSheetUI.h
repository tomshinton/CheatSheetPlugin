// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheet/Public/CheatSheetInterface.h"

#include <Runtime/Core/Public/CoreMinimal.h>

class FCheatSheetUIModule : public IModuleInterface
{
public:

	//IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~IModuleInterface
};
