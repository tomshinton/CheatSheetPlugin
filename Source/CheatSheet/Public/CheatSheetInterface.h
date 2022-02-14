// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Core/Public/Modules/ModuleInterface.h>
#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

class ICheatSheetInterface : public IModuleInterface
{

public:

	virtual void InitInternal(const APawn& InLocalPawn) = 0;
	virtual void CleanupInternal() = 0;

	virtual void RebuildCheatMap() = 0;
};