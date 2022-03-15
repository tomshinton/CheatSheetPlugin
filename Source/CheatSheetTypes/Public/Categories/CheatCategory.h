// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheetTypes/Public/Categories/CachedCheat.h"

#include "CheatCategory.generated.h"

DEFINE_LOG_CATEGORY_STATIC(CheatCategoryLog, Log, Log);

USTRUCT(BlueprintType)
struct FCheatCategory
{
	GENERATED_BODY()

public:

	CHEATSHEETTYPES_API FCheatCategory();
	CHEATSHEETTYPES_API FCheatCategory(const FString& InCategoryName, const FGuid& InParentID, const TFunction<void(const FCheatCategory&)>& InSubCatCallback);

	CHEATSHEETTYPES_API FString GetCategoryName() const;
	CHEATSHEETTYPES_API TArray<FCheatCategory> GetSubCategories() const;
	TArray<FCheatCategory>& GetSubCategoriesAsRef();
	CHEATSHEETTYPES_API FGuid GetCategoryID() const;
	CHEATSHEETTYPES_API FGuid GetParentID() const;
	CHEATSHEETTYPES_API TArray<FCachedCheat> GetCheats() const;

	void AddCheat( FCachedCheat& InCheat);

	bool operator<(const FCheatCategory& InOtherCategory) const
	{
		return CategoryName < InOtherCategory.CategoryName;
	}

private:
	
	FCheatCategory* GetExistingTopLevel(const FString& InCategoryName);

	FString CategoryName;

	FGuid ID;
	FGuid ParentID;

	TArray<FCachedCheat> Cheats;

	TArray<FCheatCategory> SubCategories;
	TFunction<void(const FCheatCategory&)> SubCatCallback;
};
