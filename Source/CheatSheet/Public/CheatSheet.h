// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include "CheatSheet/Public/CheatSheetInterface.h"

#include <CheatSheetTypes/Public/Categories/CachedCheat.h>
#include <CheatSheetTypes/Public/Categories/CheatMap.h>
#include <Runtime/Core/Public/Modules/ModuleManager.h>

#include <Runtime/CoreUObject/Public/UObject/GCObject.h>

class APlayerController;
class UCheatManager;
class UCheatSheetSettings;
class UUI_CheatSheetHome;

DECLARE_EVENT_OneParam(FCheatSheetModule, FOnCheatMapBuilt, const CheatMap&)

class FCheatSheetModule : public ICheatSheetInterface,
	public FGCObject
{
public:

	FCheatSheetModule();

	void Init(const UWorld::FActorsInitializedParams& InParams);
	void Cleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources);

	//IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~IModuleInterface

	//FGCObject
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~FGCObject

private:

	//ICheatSheetInterface
	virtual void RebuildCheatMap() override final;
	//~ICheatSheetInterface

	void CreateUI();
	void SetupBindings(APlayerController& InPlayerController);
	void SetupUINavigation();

	void ToggleListUI();
	void ShowList();
	void HideList();

#if WITH_EDITOR
	void DebugPrintCheatMap();
#endif //WITH_EDITOR

	UCheatSheetSettings* CachedSettings;
	UUI_CheatSheetHome* HomeScreen;

	TWeakObjectPtr<APlayerController> WeakPlayerController;
	TWeakObjectPtr<UCheatManager> WeakCheatManager;
	
	bool IsHomeScreenVisible;

	TOptional<CheatMap> Map;
	FOnCheatMapBuilt OnCheatMapBuilt;

	TWeakObjectPtr<UInputComponent> WeakInputComponent;
};
