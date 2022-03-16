// Retail Project - Tom Shinton 2019

#pragma once

#include <Runtime/Core/Public/Modules/ModuleInterface.h>

#include <CheatSheetTypes/Public/Categories/CachedCheat.h>

#include "CheatEntryInterface.generated.h"

class UWidget;

DECLARE_DELEGATE_OneParam(FOnCheatEntryExecuted, const FCachedCheat& /*ExecutedCheat*/);

UINTERFACE()
class UCheatEntryInterface : public UInterface 
{
	GENERATED_BODY()
};

class ICheatEntryInterface
{
	GENERATED_BODY()

public:

	virtual void ExecuteEntry() = 0;
	virtual void SetEntryNumber(const int32 InNewNumber) = 0;
	virtual void SetIsSelected(const bool InNewSelection) = 0;

	virtual const FString GetEntryTip() const = 0;
	virtual bool ShouldCloseAfterExecution() const = 0;

	virtual UWidget& GetWidget() = 0;
	
	virtual FOnCheatEntryExecuted& GetOnCheatEntryExecuted() = 0;
};

