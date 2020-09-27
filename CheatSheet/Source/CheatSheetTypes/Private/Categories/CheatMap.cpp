// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetTypes/Public/Categories/CheatMap.h"

DEFINE_LOG_CATEGORY_STATIC(CheatMapLog, Log, Log);

CheatMap::CheatMap()
	: Map()
	, FlatMap()
{

}

void CheatMap::AddCheat(FCachedCheat InCheat)
{
	if (FCheatCategory* ExistingCategory = GetExistingTopLevel(InCheat.GetTopLevelCat()))
	{
		ExistingCategory->AddCheat(InCheat);
	}
	else
	{
		FCheatCategory NewCategory(
			InCheat.GetTopLevelCat(), 
			FGuid(), 
			[this](const FCheatCategory& InNewCategory)
			{
				AddSubcategoryToFlatMap(InNewCategory);
			}
		);

		NewCategory.AddCheat(InCheat);

		Map.Add(NewCategory);
		FlatMap.Add(NewCategory.GetCategoryID(), NewCategory);
	}
}

FCheatCategory CheatMap::GetCategoryByID(const FGuid& InCategoryID) const
{
	if (FlatMap.Contains(InCategoryID))
	{
		return FlatMap[InCategoryID];
	}

	return FCheatCategory();
}

const TArray<FString> CheatMap::GetHistoryFromID(const FGuid& InID)
{
	TArray<FString> NewHistory;
	GetHistoryFromID(NewHistory, InID);

	return NewHistory;
}

void CheatMap::GetHistoryFromID(TArray<FString>& InExistingHistory, const FGuid& InID)
{
	if (FlatMap.Contains(InID))
	{
		const FCheatCategory CurrentCat = FlatMap[InID];

		InExistingHistory.Add(CurrentCat.GetCategoryName());
		GetHistoryFromID(InExistingHistory, CurrentCat.GetParentID());
	}
	else
	{
		InExistingHistory.Add(TEXT("Home"));
		Algo::Reverse(InExistingHistory);
	}
}

FCheatCategory* CheatMap::GetExistingTopLevel(const FString InCategoryName)
{
	return Map.FindByPredicate([&InCategoryName](const FCheatCategory& Category)
	{
		return Category.GetCategoryName() == InCategoryName;
	});
}

void CheatMap::AddSubcategoryToFlatMap(const FCheatCategory& InNewSubCategory)
{
	FlatMap.Add(InNewSubCategory.GetCategoryID(), InNewSubCategory);

	if (FlatMap.Contains(InNewSubCategory.GetParentID()))
	{
		FlatMap[InNewSubCategory.GetParentID()].GetSubCategoriesAsRef().Add(InNewSubCategory);
	}
}
