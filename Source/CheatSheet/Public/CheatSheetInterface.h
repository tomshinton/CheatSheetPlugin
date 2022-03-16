// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Core/Public/Modules/ModuleInterface.h>

class ICheatSheetInterface : public IModuleInterface
{
public:

	virtual void ToggleListUI() = 0;
	virtual void RebuildCheatMap() = 0;
};