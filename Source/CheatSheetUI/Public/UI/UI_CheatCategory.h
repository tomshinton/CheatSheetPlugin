// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <CheatSheetTypes/Public/Categories/CheatCategory.h>

#include "UI_CheatCategory.generated.h"

class UTextBlock;

UCLASS(hidedropdown, MinimalAPI)
class UUI_CheatCategory : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_CheatCategory(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CheatCategoryNumber;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CheatCategoryName;

	void SetCategory(const FCheatCategory& InCheatCategory) { Category = InCheatCategory; };

protected:

	void NativeConstruct() override;

private:

	FCheatCategory Category;
};
