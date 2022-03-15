// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CachedCheat.generated.h"

namespace CachedCheatStatics
{
	const FString Uncategorized = TEXT("Uncategorized");
	const FString CategoryDelim = TEXT("|");
	const FString CommonlyUsed = TEXT("_CommonlyUsed");

	const FString Error = TEXT("Error");
}

USTRUCT(BlueprintType)
struct FCachedCheat
{
	GENERATED_BODY()

public:

	FCachedCheat()
		: CheatString()
		, DisplayName(TEXT("EmptyCheat"))
		, CatConcat()
		, Tooltip(TEXT("This is an empty cheat - it will execute nothing"))
		, Func(nullptr)
	{}

	FCachedCheat(const FString& InCheatString, const FString& InDisplayName, const FString& InCatConcat, const FString& InTooltip, UFunction* InFunc)
		: CheatString(InCheatString)
		, DisplayName(InCheatString)
		, CatConcat()
		, Tooltip(InTooltip)
		, Func(InFunc)
	{
		CatConcat = ParseCategories(InCatConcat, Func);
	};

	FCachedCheat(const FString& InCheatString, const FString& InDisplayName, const TArray<FString>& InCatConcat, const FString& InTooltip, UFunction* InFunc)
		: CheatString(InCheatString)
		, DisplayName(InDisplayName)
		, CatConcat(InCatConcat)
		, Tooltip(InTooltip)
		, Func(InFunc)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CheatString;

	FString DisplayName;

	TArray<FString> CatConcat;

	FString Tooltip;

	UPROPERTY()
	UFunction* Func;

	bool operator<(const FCachedCheat& InOtherCheat) const
	{
		return DisplayName < InOtherCheat.DisplayName;
	}

public:

	FString GetTopLevelCat() const 
	{
		if (CatConcat.IsValidIndex(0))
		{
			return CatConcat[0];
		}
		else
		{
			return CachedCheatStatics::Error;
		}
	}

	void PopTopLevel()
	{
		if (CatConcat.IsValidIndex(0))
		{
			CatConcat.RemoveAt(0);
		}
	}

	bool HasValidCategories()
	{
		return CatConcat.Num() > 0;
	}

private:

	FORCEINLINE static TArray<FString> ParseCategories(const FString& InCatConcat, UFunction* InFunc = nullptr)
	{
		TArray<FString> Categories;

		if (InCatConcat.IsEmpty())
		{
			Categories.Add(CachedCheatStatics::Uncategorized);

			if (InFunc != nullptr)
			{
				Categories.Add(*InFunc->GetOuterUClass()->GetName());
			}
		}
		else
		{
			InCatConcat.ParseIntoArray(Categories, *CachedCheatStatics::CategoryDelim, true);
		}

		return Categories;
	}
};
