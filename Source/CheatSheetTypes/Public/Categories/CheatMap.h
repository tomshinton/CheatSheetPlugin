// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheetTypes/Public/Categories/CheatCategory.h"

class CheatMap
{

public:

	CHEATSHEETTYPES_API CheatMap();

	CHEATSHEETTYPES_API void AddCheat(FCachedCheat InCheat);

	CHEATSHEETTYPES_API void Reset();
	CHEATSHEETTYPES_API void Sort();
	
	TArray<FCheatCategory> GetCategories() const { return Map; }
	CHEATSHEETTYPES_API FCheatCategory GetCategoryByID(const FGuid& InID) const;
	CHEATSHEETTYPES_API TArray<FString> GetHistoryFromID(const FGuid& InID) const;

private:

	void GetHistoryFromID(TArray<FString>& InExistingString, const FGuid& InID) const;
	const FCheatCategory* GetExistingTopLevel(const FString& InCategoryName) const;
	void AddSubcategoryToFlatMap(const FCheatCategory& InNewSubCategory);

	TArray<FCheatCategory> Map;
	TMap<FGuid, FCheatCategory> FlatMap;
};