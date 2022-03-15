// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/DeveloperSettings/Public/Engine/DeveloperSettings.h>
#include <Runtime/Engine/Classes/GameFramework/PlayerInput.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "CheatSheetSettings.generated.h"

namespace CheatSheetBindingNames
{
	const FName ShowBinding = TEXT("CS_SHOW");
	const FName ConfirmBinding = TEXT("CS_CONFIRM");
	const FName UpBinding = TEXT("CS_UP");
	const FName DownBinding = TEXT("CS_DOWN");
	const FName BackBinding = TEXT("CS_BACK");
};

class UUI_CheatSheetHome;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Cheat Sheet"))
class CHEATSHEET_API UCheatSheetSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UCheatSheetSettings(const FObjectInitializer& InObjectInitialiser)
		: Super(InObjectInitialiser)
		, CheatSheetMenu(nullptr)
		, ShowBinding()
		, ConfirmBinding()
		, UpBinding()
		, DownBinding()
		, BackBinding()
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_CheatSheetHome> CheatSheetMenu;

	UPROPERTY(config, EditDefaultsOnly)
	FInputActionKeyMapping ShowBinding;

	UPROPERTY(config, EditDefaultsOnly)
	FInputActionKeyMapping ConfirmBinding;

	UPROPERTY(config, EditDefaultsOnly)
	FInputActionKeyMapping UpBinding;

	UPROPERTY(config, EditDefaultsOnly)
	FInputActionKeyMapping DownBinding;

	UPROPERTY(config, EditDefaultsOnly)
	FInputActionKeyMapping BackBinding;

	//UDeveloperSettings
	virtual FName GetCategoryName() const override { return FName(TEXT("Plugins")); }
	//~UDeveloperSettings

	FInputActionKeyMapping GetShowBinding()
	{
		ShowBinding.ActionName = CheatSheetBindingNames::ShowBinding;
		return ShowBinding;
	}

	FInputActionKeyMapping GetConfirmBinding()
	{
		ConfirmBinding.ActionName = CheatSheetBindingNames::ConfirmBinding;
		return ConfirmBinding;
	}

	FInputActionKeyMapping GetUpBinding()
	{
		UpBinding.ActionName = CheatSheetBindingNames::UpBinding;
		return UpBinding;
	}

	FInputActionKeyMapping GetDownBinding() 
	{
		DownBinding.ActionName = CheatSheetBindingNames::DownBinding;
		return DownBinding;
	}

	FInputActionKeyMapping GetBackBinding() 
	{
		BackBinding.ActionName = CheatSheetBindingNames::BackBinding;
		return BackBinding;
	}
};

