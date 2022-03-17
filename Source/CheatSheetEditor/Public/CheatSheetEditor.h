// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Core/Public/Modules/ModuleManager.h>

#include <Developer/AssetTools/Public/IAssetTools.h>
#include <Runtime/SlateCore/Public/Styling/SlateStyle.h>

class FCheatSheetEditorModule : public IModuleInterface
{

public:

	FCheatSheetEditorModule();

	//IModuleInterface
	void StartupModule() override;
	void ShutdownModule() override;
	//~IModuleInterface

private:

	void RegisterNewAssetCategory();
	void RegisterNewAssetIcon();

	TSharedPtr<FSlateStyleSet> StyleSet;
};
