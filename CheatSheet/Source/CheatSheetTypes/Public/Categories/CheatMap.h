// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheetTypes/Public/Categories/CheatCategory.h"

class CheatMap
{

public:

	CHEATSHEETTYPES_API CheatMap();

	//Deliberately take this in as a copy
	CHEATSHEETTYPES_API void AddCheat(FCachedCheat InCheat);

	const TArray<FCheatCategory> GetCategories() const { return Map; }
	CHEATSHEETTYPES_API FCheatCategory GetCategoryByID(const FGuid& InID) const;

	CHEATSHEETTYPES_API const TArray<FString> GetHistoryFromID(const FGuid& InID);

private:

	void GetHistoryFromID(TArray<FString>& InExistingString, const FGuid& InID);
	FCheatCategory* GetExistingTopLevel(const FString InCategoryName);
	void AddSubcategoryToFlatMap(const FCheatCategory& InNewSubCategory);

	TArray<FCheatCategory> Map;
	TMap<FGuid, FCheatCategory> FlatMap;
};