// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/UI_CheatSheetHome.h"
#include "CheatSheetUI/Public/UI/UI_CheatView.h"
#include "CheatSheetUI/Public/UI/Tips/UI_ControlTip.h"
#include "CheatSheetUI/Public/UI/Tips/UI_CurrentTooltip.h"

#include <Runtime/Core/Public/Modules/ModuleManager.h>
#include <Runtime/UMG/Public/Components/SizeBox.h>
#include <Runtime/UMG/Public/Components/TextBlock.h>
#include <Runtime/UMG/Public/Components/VerticalBox.h>

#include "CheatSheet/Public/CheatSheet.h"

UUI_CheatSheetHome::UUI_CheatSheetHome(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CheatView(nullptr)
	, HistoryReadout(nullptr)
	, ControlTipsBox(nullptr)
	, CurrentTooltipBox(nullptr)
	, InfoBox(nullptr)
	, Map()
	, History()
	, CurrentCategory(FCheatCategory())
{}

void UUI_CheatSheetHome::AddMap(const CheatMap& InMap)
{
	Map = InMap;
	InitHomeView();
}

void UUI_CheatSheetHome::InitHomeView()
{
	History.Empty();
	CheatView->OnNewCategoryRequested.RemoveAll(this);

	if (CheatView != nullptr)
	{
		CheatView->OnNewCategoryRequested.AddUObject(this, &UUI_CheatSheetHome::UpdateHistoryReadout);

		if (CurrentTooltipBox != nullptr)
		{
			CheatView->OnNewSelection.AddWeakLambda(this, [this](const TWeakInterfacePtr<ICheatEntryInterface>& InOldSelection, const TWeakInterfacePtr<ICheatEntryInterface>& InNewSelection)
			{
				if (ICheatEntryInterface* NewSelection = InNewSelection.Get())
				{
					if (ICheatEntryInterface* OldSelection = InOldSelection.Get())
					{
						OldSelection->GetOnCheatEntryExecuted().Unbind();
					}

					NewSelection->GetOnCheatEntryExecuted().BindWeakLambda(this, [this, NewSelection = NewSelection](const FCachedCheat& InCheat)
					{
						if(InCheat.ShouldCloseAfterExecution)
						{
							ICheatSheetInterface& CheatSheet = FModuleManager::GetModuleChecked<ICheatSheetInterface>("CheatSheet");
							CheatSheet.ToggleListUI();
						}
					});

					const FString EntryTip = NewSelection->GetEntryTip();
					if(!EntryTip.IsEmpty())
					{
						InfoBox->SetVisibility(ESlateVisibility::Visible);
						CurrentTooltipBox->SetTooltip(NewSelection->GetEntryTip());
					}
					else
					{
						InfoBox->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			});
		}

		if (Map.IsSet())
		{
			CheatView->RequestHomeView(Map.GetValue().GetCategories());

			History.Add(TEXT("Home"));
			if (HistoryReadout != nullptr)
			{
				HistoryReadout->SetText(FText::FromString(GetHistoryAsString(History)));
			}
		}
	}
}

void UUI_CheatSheetHome::ShowPreviousCategory()
{
	if(History.Num() > 1)
	{
		ShowScreenByID(CurrentCategory.GetParentID());
	}
	else
	{
		ICheatSheetInterface& CheatSheet = FModuleManager::GetModuleChecked<ICheatSheetInterface>("CheatSheet");
		CheatSheet.ToggleListUI();
	}
}

bool UUI_CheatSheetHome::ShowScreenByID(const FGuid& InID)
{
	if (Map.IsSet())
	{
		const FCheatCategory FoundCategory = Map.GetValue().GetCategoryByID(InID);

		if (FoundCategory.GetCategoryID().IsValid())
		{
			CheatView->RequestViewForCategory(FoundCategory);
			return true;
		}
		else
		{
			InitHomeView();
		}
	}
	else
	{
		InitHomeView();
	}
	
	return false;
}

void UUI_CheatSheetHome::UpdateHistoryReadout(const FCheatCategory& InLastRequestedCategory)
{
	if (Map.IsSet())
	{
		CurrentCategory = InLastRequestedCategory;
		History = Map.GetValue().GetHistoryFromID(InLastRequestedCategory.GetCategoryID());

		if (HistoryReadout != nullptr)
		{
			HistoryReadout->SetText(FText::FromString(GetHistoryAsString(History)));
		}
	}
}

FString UUI_CheatSheetHome::GetHistoryAsString(const TArray<FString>& InHistory)
{
	FString HistoryAsString;

	for (const FString Category : InHistory)
	{
		HistoryAsString += FString::Printf(TEXT("/%s"), *Category);
	}

	return HistoryAsString;
}
