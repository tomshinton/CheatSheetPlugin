// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/UI_CheatView.h"

#include "CheatSheetUI/Public/UI/Entries/UI_CategoryEntry.h"
#include "CheatSheetUI/Public/UI/Entries/UI_CheatEntry.h"

#include <Runtime/UMG/Public/Components/VerticalBox.h>

DEFINE_LOG_CATEGORY_STATIC(CheatViewLog, Log, Log);

UUI_CheatView::UUI_CheatView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CategoryBox(nullptr)
	, CheatBox(nullptr)
	, CheatCategoryClass(nullptr)
	, CheatEntryClass(nullptr)
	, Categories()
	, OnNewCategoryRequested()
	, OnNewSelection()
	, RequestCallback(nullptr)
	, Entries()
	, CurrentSelection()
	, CurrentSelectionIndex(0)
	, TotalEntries(0)
{}

void UUI_CheatView::NativeConstruct()
{
	Super::NativeConstruct();

	RequestCallback = [WeakThis = TWeakObjectPtr<UUI_CheatView>(this), this](const FCheatCategory& InRequestingCategory)
	{
		if (WeakThis.IsValid())
		{
			RequestViewForCategory(InRequestingCategory);
		}
	};
}

void UUI_CheatView::RequestViewForCategory(const FCheatCategory& InCheatCategory)
{
	TotalEntries = 0;
	CurrentSelectionIndex = 0;
 
	if (CategoryBox != nullptr && CheatBox != nullptr)
	{
		if (CheatCategoryClass != nullptr && CheatEntryClass != nullptr)
		{
			CategoryBox->ClearChildren();
			CheatBox->ClearChildren();

			const TArray<FCheatCategory> CachedCategories = InCheatCategory.GetSubCategories();
			for (const FCheatCategory& Category : CachedCategories)
			{
				UUI_CategoryEntry* NewCategory = CreateWidget<UUI_CategoryEntry>(this, CheatCategoryClass);
				NewCategory->SetCategory(Category);
				NewCategory->SetRequestCallback(RequestCallback);

				CategoryBox->AddChild(NewCategory);
			}

			const TArray<FCachedCheat> CachedCheats = InCheatCategory.GetCheats();
			for (const FCachedCheat& Cheat : CachedCheats)
			{
				UUI_CheatEntry* NewCheat = CreateWidget<UUI_CheatEntry>(this, CheatEntryClass);
				NewCheat->SetCheat(Cheat);
				CheatBox->AddChild(NewCheat);
			}
		}
	}

	OnNewCategoryRequested.Broadcast(InCheatCategory);

	ApplyEntryNumbers();
	ApplyNewSelectionIndex();
}

void UUI_CheatView::RequestHomeView(const TArray<FCheatCategory>& InCheatCategories)
{
	TotalEntries = 0;
	CurrentSelectionIndex = 0;

	if (CheatCategoryClass != nullptr)
	{
		if (CheatCategoryClass != nullptr && CheatEntryClass != nullptr)
		{
			CategoryBox->ClearChildren();
			CheatBox->ClearChildren();

			for (const FCheatCategory& Category : InCheatCategories)
			{
				UUI_CategoryEntry* NewCategory = CreateWidget<UUI_CategoryEntry>(this, CheatCategoryClass);
				NewCategory->SetCategory(Category);
				NewCategory->SetRequestCallback(RequestCallback);

				CategoryBox->AddChild(NewCategory);
			}
		}
	}

	ApplyEntryNumbers();
	ApplyNewSelectionIndex();
}

void UUI_CheatView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	RequestHomeView(Categories);
}

void UUI_CheatView::ConfirmSelection()
{
	if (CurrentSelection.IsValid())
	{
		CurrentSelection->ExecuteEntry();
	}
}

void UUI_CheatView::UpSelection()
{
	CurrentSelectionIndex == 0 ? CurrentSelectionIndex = TotalEntries - 1 : CurrentSelectionIndex--;
	ApplyNewSelectionIndex();
}

void UUI_CheatView::DownSelection()
{
	CurrentSelectionIndex == TotalEntries - 1 ? CurrentSelectionIndex = 0 : CurrentSelectionIndex++;
	ApplyNewSelectionIndex();
}

void UUI_CheatView::ApplyEntryNumbers()
{
	if (CheatBox != nullptr && CategoryBox != nullptr)
	{
		for (int32 i = 0; i < CategoryBox->GetChildrenCount(); ++i)
		{
			UWidget* Child = CategoryBox->GetChildAt(i);
			if (ICheatEntryInterface* Entry = Cast<ICheatEntryInterface>(Child))
			{
				Entry->SetEntryNumber(TotalEntries);
				Entries.Add(TotalEntries, *Entry);

				TotalEntries++;
			}
		}

		for (int32 i = 0; i < CheatBox->GetChildrenCount(); ++i)
		{
			UWidget* Child = CheatBox->GetChildAt(i);
			if (ICheatEntryInterface* Entry = Cast<ICheatEntryInterface>(Child))
			{
				Entry->SetEntryNumber(TotalEntries);
				Entries.Add(TotalEntries, *Entry);

				TotalEntries++;
			}
		}
	}
}

void UUI_CheatView::ApplyNewSelectionIndex()
{
	if (CurrentSelection.IsValid()) //Deselect old entry
	{
		CurrentSelection->SetIsSelected(false);
	}

	CurrentSelection = Entries[CurrentSelectionIndex];

	if (CurrentSelection.IsValid()) //Select new entry
	{
		CurrentSelection->SetIsSelected(true);
		OnNewSelection.Broadcast(CurrentSelection);
	}
}