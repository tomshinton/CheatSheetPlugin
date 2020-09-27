// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Object.h>
#include <CheatSheetTypes/Public/Categories/CachedCheat.h>

#include "CachedCheatAsset.generated.h"

DEFINE_LOG_CATEGORY_STATIC(CachedCheatAssetLog, Log, Log);

UCLASS(BlueprintType, Blueprintable)
class CHEATSHEETTYPES_API UCachedCheatAsset : public UObject
{
	GENERATED_BODY()

public:

	UCachedCheatAsset()
		: CheatString()
		, Categories()
		, IsCommonlyUsed(false)
		, Tooltip()
		, DisplayName()
	{};

	FCachedCheat GetCheat() const
	{
		TArray<FString> CleanCategories = Categories;
		if (CleanCategories.Num() > 0)
		{
			for (int32 i = CleanCategories.Num() -1; i >= 0; --i)
			{
				if (CleanCategories[i].IsEmpty())
				{
					CleanCategories.RemoveAt(i);
				}
			}
		}
		else
		{
			CleanCategories.Add(CachedCheatStatics::Uncategorized);
		}

		if (IsCommonlyUsed)
		{
			CleanCategories.Insert(CachedCheatStatics::CommonlyUsed, 0);
		}
			
		return FCachedCheat(CheatString, DisplayName, CleanCategories, Tooltip, nullptr);
	};


	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat", Tooltip = "The cheat itself, matches syntax of a console command.  Unlike autodiscovery from the CheatManager, this will support params, so Game.DoYourThing 1 is valid here"))
	FString CheatString;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Categories in descending order.  For Instance, Root/Category1/Category2"))
	TArray<FString> Categories;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Does this Cheat need to be added to the Commonly Used top level category? Typically high traffic cheats"))
	bool IsCommonlyUsed;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "What does this cheat actually do?"))
	FString Tooltip;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Readable cheat name"))
	FString DisplayName;
};
