// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <CheatSheetTypes/Public/Categories/CheatCategory.h>
#include <CheatSheetTypes/Public/Categories/CheatMap.h>

#include "UI_CheatSheetHome.generated.h"

class UUI_CurrentTooltip;
class UUI_ControlTip;
class UUI_CheatView;
class UVerticalBox;
class UTextBlock;

UCLASS(hidedropdown, MinimalAPI)
class UUI_CheatSheetHome : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_CheatSheetHome(const FObjectInitializer& ObjectInitializer);

	CHEATSHEETUI_API void AddMap(const CheatMap& InMap);
	CHEATSHEETUI_API UUI_CheatView* GetCheatView() const { return CheatView; }

	CHEATSHEETUI_API void InitHomeView();

	CHEATSHEETUI_API void ShowPreviousCategory();

	bool ShowScreenByID(const FGuid& InID);

protected:

	UPROPERTY(meta = (BindWidget))
	UUI_CheatView* CheatView;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HistoryReadout;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ControlTipsBox;

	UPROPERTY(meta = (BindWidget))
	UUI_CurrentTooltip* CurrentTooltipBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Categories")
	TSubclassOf<UUI_ControlTip> ControlTipClass;

private:

	void SynchronizeProperties() override;

	void UpdateHistoryReadout(const FCheatCategory& InLastRequestedCategory);
	static FString GetHistoryAsString(const TArray<FString>& InHistory);

	TOptional<CheatMap> Map;
	TArray<FString> History;

	FCheatCategory CurrentCategory;
};
