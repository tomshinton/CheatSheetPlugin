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
		, ShowBindings()
		, ConfirmBindings()
		, UpBindings()
		, DownBindings()
		, BackBindings()
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_CheatSheetHome> CheatSheetMenu;

	UPROPERTY(config, EditDefaultsOnly)
	TArray<FInputActionKeyMapping> ShowBindings;

	UPROPERTY(config, EditDefaultsOnly)
	TArray<FInputActionKeyMapping> ConfirmBindings;

	UPROPERTY(config, EditDefaultsOnly)
	TArray<FInputActionKeyMapping> UpBindings;

	UPROPERTY(config, EditDefaultsOnly)
	TArray<FInputActionKeyMapping> DownBindings;

	UPROPERTY(config, EditDefaultsOnly)
	TArray<FInputActionKeyMapping> BackBindings;

	//UDeveloperSettings
	virtual FName GetCategoryName() const override { return FName(TEXT("Plugins")); }
	//~UDeveloperSettings

	TArray<FInputActionKeyMapping> GetShowBinding()
	{
		for(FInputActionKeyMapping& Mapping : ShowBindings)
		{
			Mapping.ActionName = CheatSheetBindingNames::ShowBinding;
		}
		
		return ShowBindings;
	}

	TArray<FInputActionKeyMapping> GetConfirmBinding()
	{
		for(FInputActionKeyMapping& Mapping : ConfirmBindings)
		{
			Mapping.ActionName = CheatSheetBindingNames::ConfirmBinding;
		}

		return ConfirmBindings;
	}

	TArray<FInputActionKeyMapping> GetUpBinding()
	{
		for(FInputActionKeyMapping& Mapping : UpBindings)
		{
			Mapping.ActionName = CheatSheetBindingNames::UpBinding;
		}

		return UpBindings;
	}

	TArray<FInputActionKeyMapping> GetDownBinding() 
	{
		for(FInputActionKeyMapping& Mapping : DownBindings)
		{
			Mapping.ActionName = CheatSheetBindingNames::DownBinding;
		}

		return DownBindings;
	}

	TArray<FInputActionKeyMapping> GetBackBinding() 
	{
		for(FInputActionKeyMapping& Mapping : BackBindings)
		{
			Mapping.ActionName = CheatSheetBindingNames::BackBinding;
		}
		
		return BackBindings;
	}
};

