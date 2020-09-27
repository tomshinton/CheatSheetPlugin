// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetEditor/Public/Actions/CachedCheatBuilderActions.h"
#include "CheatSheetEditor/Public/CheatSheetEditor.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatBuilder.h>

#define LOCTEXT_NAMESPACE "CheatSheetAssetActions"

FCachedCheatBuilderActions::FCachedCheatBuilderActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{}

bool FCachedCheatBuilderActions::CanFilter()
{
	return true;
}

uint32 FCachedCheatBuilderActions::GetCategories()
{
	return AssetCategory;
}

FText FCachedCheatBuilderActions::GetName() const
{
	return LOCTEXT("CachedCheatAsset", "Cached Cheat Builder");
}

UClass* FCachedCheatBuilderActions::GetSupportedClass() const
{
	return UCachedCheatBuilder::StaticClass();
}

FColor FCachedCheatBuilderActions::GetTypeColor() const
{
	return FColorList::Scarlet;
}

bool FCachedCheatBuilderActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE