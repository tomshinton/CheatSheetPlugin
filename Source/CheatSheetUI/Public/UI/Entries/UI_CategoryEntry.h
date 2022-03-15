// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheetUI/Public/UI/Entries/UI_CheatEntryBase.h"
#include <CheatSheetTypes/Public/Categories/CheatCategory.h>

#include "UI_CategoryEntry.generated.h"

UCLASS(hidedropdown, MinimalAPI)
class UUI_CategoryEntry : public UUI_CheatEntryBase
{
	GENERATED_BODY()

public:

	UUI_CategoryEntry(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumSubCategoriesText;

	void SetCategory(const FCheatCategory& InCheatCategory) { Category = InCheatCategory; };
	void SetRequestCallback(const TFunction<void(const FCheatCategory&)>& InRequestCallback) { RequestCallback = InRequestCallback; }

	//ICheatEntryInterface
	virtual void ExecuteEntry() override;
	//~ICheatEntryInterface

protected:

	void NativeConstruct() override;

private:

	FCheatCategory Category;
	TFunction<void(const FCheatCategory&)> RequestCallback;
};
