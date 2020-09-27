// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheetUI/Public/UI/Entries/UI_CheatEntryBase.h"

#include <CheatSheetTypes/Public/Categories/CachedCheat.h>
#include <Runtime/SlateCore/Public/Styling/SlateBrush.h>

#include "UI_CheatEntry.generated.h"

class UTextBlock;

UCLASS(hidedropdown, MinimalAPI)
class UUI_CheatEntry : public UUI_CheatEntryBase
{
	GENERATED_BODY()

public:

	UUI_CheatEntry(const FObjectInitializer& ObjectInitializer);

	void SetCheat(const FCachedCheat& InCheat) { Cheat = InCheat; }

	//ICheatEntryInterface
	void ExecuteEntry() override;
	virtual const FString GetEntryTip() const override;
	//~ICheatEntryInterface
	
protected:

	void NativeConstruct() override;

private:

	FCachedCheat Cheat;
};
