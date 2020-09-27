// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetEditor/Public/CheatSheetEditor.h"
#include "CheatSheetEditor/Public/Actions/CachedCheatAssetActions.h"
#include "CheatSheetEditor/Public/Actions/CachedCheatBuilderActions.h"

#include <CheatSheet/Public/CheatSheetInterface.h>
#include <CheatSheetTypes/Public/Categories/CachedCheatAsset.h>
#include <CheatSheetTypes/Public/Categories/CachedCheatBuilder.h>
#include <Developer/AssetTools/Public/AssetToolsModule.h>
#include <Developer/AssetTools/Public/AssetTypeCategories.h>
#include <Runtime/Projects/Public/Interfaces/IPluginManager.h>
#include <Runtime/SlateCore/Public/Styling/SlateStyleRegistry.h>

IMPLEMENT_MODULE(FCheatSheetEditorModule, CheatSheetEditor);

#define LOCTEXT_NAMESPACE "CheatSheetEditor"

FCheatSheetEditorModule::FCheatSheetEditorModule()
{
}

void FCheatSheetEditorModule::StartupModule()
{
	RegisterNewAssetCategory();
	RegisterNewAssetIcon();

	if (ICheatSheetInterface* CheatSheetInterface = FModuleManager::GetModulePtr<ICheatSheetInterface>("CheatSheet"))
	{
		FCoreUObjectDelegates::OnObjectSaved.AddLambda([CheatSheetInterface](UObject* InNewObject)
		{
			if (Cast<UCachedCheatAsset>(InNewObject) || Cast<UCachedCheatBuilder>(InNewObject))
			{
				CheatSheetInterface->RebuildCheatMap();
			}
		});

		FEditorDelegates::OnAssetsDeleted.AddLambda([CheatSheetInterface](const TArray<UClass*>& InDeletedClasses)
		{
			if (InDeletedClasses.Contains(UCachedCheatAsset::StaticClass()) || InDeletedClasses.Contains(UCachedCheatBuilder::StaticClass()))
			{
				CheatSheetInterface->RebuildCheatMap();
			}
		});
	}
}

void FCheatSheetEditorModule::ShutdownModule()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FCheatSheetEditorModule::RegisterNewAssetCategory()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type CheatSheetCategoryBit = CheatSheetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("CheatSheet")), LOCTEXT("CheatSheetCategory", "Cheat Sheet"));

	TSharedPtr<IAssetTypeActions> CachedCheatAction = MakeShareable(new FCachedCheatAssetActions(CheatSheetCategoryBit));
	AssetTools.RegisterAssetTypeActions(CachedCheatAction.ToSharedRef());

	TSharedPtr<IAssetTypeActions> CachedCheatBuilderAction = MakeShareable(new FCachedCheatBuilderActions(CheatSheetCategoryBit));
	AssetTools.RegisterAssetTypeActions(CachedCheatBuilderAction.ToSharedRef());
}

void FCheatSheetEditorModule::RegisterNewAssetIcon()
{
	StyleSet = MakeShareable(new FSlateStyleSet("CheatSheetStyle"));

	FString ContentDir = IPluginManager::Get().FindPlugin("CheatSheet")->GetBaseDir();
	StyleSet->SetContentRoot(ContentDir);

	FSlateImageBrush* CachedCheatBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/Icon64"), TEXT(".png")), FVector2D(64.f, 64.f));
	FSlateImageBrush* CachedCheatBuilderBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/BuilderIcon64"), TEXT(".png")), FVector2D(64.f, 64.f));

	if (CachedCheatBrush && CachedCheatBuilderBrush)
	{
		StyleSet->Set("ClassThumbnail.CachedCheatAsset", CachedCheatBrush);
		StyleSet->Set("ClassThumbnail.CachedCheatBuilder", CachedCheatBuilderBrush);
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

#undef LOCTEXT_NAMESPACE