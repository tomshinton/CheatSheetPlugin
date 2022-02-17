// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/DeveloperSettings/Public/Engine/DeveloperSettings.h>
#include <Runtime/Engine/Classes/GameFramework/PlayerInput.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "CheatSheetSettings.generated.h"

class UUI_CheatSheetHome;

namespace BindingNames
{
	const FName Show = TEXT("CHEATSHEET_SHOW");
	const FName Confirm = TEXT("CHEATSHEET_CONFIRM");
	const FName Up = TEXT("CHEATSHEET_Up");
	const FName Down = TEXT("CHEATSHEET_Down");
	const FName Back = TEXT("CHEATSHEET_Back");
}

UCLASS(config = CheatSheet, defaultconfig, meta = (DisplayName = "CheatSheet"))
class UCheatSheetSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UCheatSheetSettings()
		: CheatSheetMenu(nullptr)
		, ShowMenuBinding(BindingNames::Show)
		, ConfirmBinding(BindingNames::Confirm)
		, UpBinding(BindingNames::Up)
		, DownBinding(BindingNames::Down)
		, BackBinding(BindingNames::Back)
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_CheatSheetHome> CheatSheetMenu;

	UPROPERTY(config, EditDefaultsOnly, Category = "Show Menu Binding")
	FInputActionKeyMapping ShowMenuBinding;

	UPROPERTY(config, EditDefaultsOnly, Category = "Confirm Binding")
	FInputActionKeyMapping ConfirmBinding;

	UPROPERTY(config, EditDefaultsOnly, Category = "Up Binding")
	FInputActionKeyMapping UpBinding;

	UPROPERTY(config, EditDefaultsOnly, Category = "Down Binding")
	FInputActionKeyMapping DownBinding;

	UPROPERTY(config, EditDefaultsOnly, Category = "Back Binding")
	FInputActionKeyMapping BackBinding;

	FName GetCategoryName() const override
	{
		return FName(TEXT("Plugins"));
	}
};