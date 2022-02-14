// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Object.h>
#include <CheatSheetTypes/Public/Categories/CachedCheat.h>

#include "CachedCheatBuilder.generated.h"

DEFINE_LOG_CATEGORY_STATIC(CachedCheatBuilderLog, Log, Log);

UCLASS(BlueprintType, Blueprintable)
class CHEATSHEETTYPES_API UCachedCheatBuilder : public UObject
{
	GENERATED_BODY()

public:

	UCachedCheatBuilder();
	
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Categories in descending order.  For Instance, Root/Category1/Category2."))
	TArray<FString> Categories;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Base cheat string.  This is the string that will have any dynamically generated params appended onto it."))
	FString CheatStringRoot;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "Does this Cheat need to be added to the Commonly Used top level category? Typically high traffic cheats.  As this is a dynamically generated cheat, having a category is highly advised."))
	bool IsCommonlyUsed;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Cheat Meta", Tooltip = "What does this cheat actually do?"))
	FString Tooltip;

	UFUNCTION(BlueprintImplementableEvent, Category = "Generation")
	TArray<FString> Generate();
};
