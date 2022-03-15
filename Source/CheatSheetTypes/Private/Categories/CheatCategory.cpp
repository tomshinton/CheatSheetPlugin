#pragma once 

#include "CheatSheetTypes/Public/Categories/CheatCategory.h"

FCheatCategory::FCheatCategory() 
	: CategoryName(TEXT("Empty Category"))
	, ID(FGuid())
	, ParentID(FGuid())
	, Cheats()
	, SubCategories()
	, SubCatCallback(nullptr)
{}

FCheatCategory::FCheatCategory(const FString& InCategoryName, const FGuid& InParentID, const TFunction<void(const FCheatCategory&)>& InSubCatCallback) 
	: CategoryName(InCategoryName)
	, ID(FGuid::NewGuid())
	, ParentID(InParentID)
	, Cheats()
	, SubCategories()
	, SubCatCallback(InSubCatCallback)
{}

FString FCheatCategory::GetCategoryName() const
{
	return CategoryName;
}

TArray<FCheatCategory> FCheatCategory::GetSubCategories() const
{
	return SubCategories;
}

TArray<FCheatCategory>& FCheatCategory::GetSubCategoriesAsRef()
{
	return SubCategories;
}

FGuid FCheatCategory::GetCategoryID() const
{
	return ID;
}

FGuid FCheatCategory::GetParentID() const
{
	return ParentID;
}

TArray<FCachedCheat> FCheatCategory::GetCheats() const
{
	return Cheats;
}

void FCheatCategory::AddCheat(FCachedCheat& InCheat)
{
	//Remove nested categories like Test/Test
	if (InCheat.GetTopLevelCat() == CategoryName)
	{
		InCheat.PopTopLevel();
	}

	//Are we at the bottom of the list? If so, add the cheat here
	if (!InCheat.HasValidCategories())
	{
		Cheats.Add(InCheat);
		Cheats.Sort();

		return;
	}
	else
	{
		if (FCheatCategory* ExistingCategory = GetExistingTopLevel(InCheat.GetTopLevelCat()))
		{
			ExistingCategory->AddCheat(InCheat);
		}
		else
		{
			FCheatCategory NewCategory(InCheat.GetTopLevelCat(), ID, SubCatCallback);
			NewCategory.AddCheat(InCheat);
			SubCategories.Add(NewCategory);

			if (SubCatCallback != nullptr)
			{
				SubCatCallback(NewCategory);
			}
		}
	}
}

FCheatCategory* FCheatCategory::GetExistingTopLevel(const FString& InCategoryName)
{
	return SubCategories.FindByPredicate([&InCategoryName](const FCheatCategory& Category)
	{
		return Category.GetCategoryName() == InCategoryName;
	});
}
