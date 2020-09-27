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
	CHEATSHEETTYPES_API FCheatCategory(const FString InCategoryName, const FGuid& InParentID, const TFunction<void(const FCheatCategory&)>& InSubCatCallback);

	CHEATSHEETTYPES_API const FString GetCategoryName() const;
	CHEATSHEETTYPES_API const TArray<FCheatCategory> GetSubCategories() const;
	CHEATSHEETTYPES_API TArray<FCheatCategory>& GetSubCategoriesAsRef();
	CHEATSHEETTYPES_API const FGuid GetCategoryID() const;
	CHEATSHEETTYPES_API const FGuid GetParentID() const;
	CHEATSHEETTYPES_API const TArray<FCachedCheat> GetCheats() const;

	void AddCheat( FCachedCheat& InCheat);

#if WITH_EDITOR
	CHEATSHEETTYPES_API void Print(const uint8 Depth) const;
#endif //WITH_EDITOR

private:
	
	FCheatCategory* GetExistingTopLevel(const FString InCategoryName);

	FString CategoryName;

	FGuid ID;
	FGuid ParentID;

	TArray<FCachedCheat> Cheats;

	TArray<FCheatCategory> SubCategories;
	TFunction<void(const FCheatCategory&)> SubCatCallback;
};
