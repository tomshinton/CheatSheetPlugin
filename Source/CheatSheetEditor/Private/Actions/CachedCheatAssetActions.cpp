// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetEditor/Public/Actions/CachedCheatAssetActions.h"
#include "CheatSheetEditor/Public/CheatSheetEditor.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatAsset.h>

#define LOCTEXT_NAMESPACE "CheatSheetAssetActions"

FCachedCheatAssetActions::FCachedCheatAssetActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{}

bool FCachedCheatAssetActions::CanFilter()
{
	return true;
}

uint32 FCachedCheatAssetActions::GetCategories()
{
	return AssetCategory;
}

FText FCachedCheatAssetActions::GetName() const
{
	return LOCTEXT("CachedCheatAsset", "Cached Cheat");
}

UClass* FCachedCheatAssetActions::GetSupportedClass() const
{
	return UCachedCheatAsset::StaticClass();
}

FColor FCachedCheatAssetActions::GetTypeColor() const
{
	return FColorList::Scarlet;
}

bool FCachedCheatAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE