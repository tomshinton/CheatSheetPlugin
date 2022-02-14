// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/DeveloperSettings/Public/Engine/DeveloperSettings.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "CheatSheetSettings.generated.h"

class UUI_CheatSheetHome;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "CheatSheet"))
class UCheatSheetSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UCheatSheetSettings()
		: CheatSheetMenu(nullptr)
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_CheatSheetHome> CheatSheetMenu;

	FName GetCategoryName() const override
	{
		return FName(TEXT("Plugins"));
	}
};