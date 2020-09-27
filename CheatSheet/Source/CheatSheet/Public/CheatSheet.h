// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include "CheatSheet/Public/CheatSheetInterface.h"

#include <CheatSheetTypes/Public/Categories/CachedCheat.h>
#include <CheatSheetTypes/Public/Categories/CheatMap.h>
#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Core/Public/Modules/ModuleManager.h>
#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

class APlayerController;
class UCheatManager;
class UCheatSheetSettings;
class UUI_CheatSheetHome;

DECLARE_EVENT_OneParam(FCheatSheetModule, FOnCheatMapBuilt, const CheatMap&)

class FCheatSheetModule : public ICheatSheetInterface
{
public:

	FCheatSheetModule();

	CHEATSHEET_API static void Init(const APawn& InLocalPlayer);
	CHEATSHEET_API static void Cleanup();

	//IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~IModuleInterface

private:

	//ICheatSheetInterface
	virtual void InitInternal(const APawn& InLocalPawn) override;
	virtual void CleanupInternal() override final;
	virtual void RebuildCheatMap() override final;
	//~ICheatSheetInterface

	void CacheSettings();
	void PreCreateUI();
	void SetupBindings();

	//UI
	void ToggleListUI();
	void ShowList();
	void HideList();

#if WITH_EDITOR
	void DebugPrintCheatMap();
#endif //WITH_EDITOR

	UPROPERTY()
	UCheatSheetSettings* CachedSettings;

	UPROPERTY()
	APlayerController* CachedPlayerController;

	UPROPERTY()
	TWeakObjectPtr<UUI_CheatSheetHome> HomeScreen; //This is weak as it may of been using a previous PIE PlayerController to keep it alive - if it's out of date of stale, recreate

	UPROPERTY()
	TWeakObjectPtr<UCheatManager> CurrentCheatManager;

	bool IsHomeScreenVisible;

	TOptional<CheatMap> Map;
	FOnCheatMapBuilt OnCheatMapBuilt;
};
