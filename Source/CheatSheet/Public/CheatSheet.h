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

	void Init(const UWorld::FActorsInitializedParams& InParams);
	void Cleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources);

	//IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~IModuleInterface

private:

	//ICheatSheetInterface
	virtual void RebuildCheatMap() override final;
	//~ICheatSheetInterface

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
	const UCheatSheetSettings* CachedSettings;

	TWeakObjectPtr<APlayerController> WeakPlayerController;
	TWeakObjectPtr<UCheatManager> WeakCheatManager;

	UPROPERTY()
	TWeakObjectPtr<UUI_CheatSheetHome> HomeScreen;

	bool IsHomeScreenVisible;

	TOptional<CheatMap> Map;
	FOnCheatMapBuilt OnCheatMapBuilt;
};
