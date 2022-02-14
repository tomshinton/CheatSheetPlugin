// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>

#include "CheatSheetUI/Public/UI/CheatEntryInterface.h"

#include "UI_CheatView.generated.h"

struct FCheatCategory;

class UUI_CategoryEntry;
class UUI_CheatEntry;
class UVerticalBox;

DECLARE_EVENT_OneParam(UUI_CheatView, FOnNewCategoryRenderRequested, const FCheatCategory&)
DECLARE_EVENT_OneParam(UUI_CheatView, FOnNewSelectiom, const TWeakInterfacePtr<ICheatEntryInterface>&)

UCLASS(hidedropdown, MinimalAPI)
class UUI_CheatView : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_CheatView(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CategoryBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CheatBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Categories")
	TSubclassOf<UUI_CategoryEntry> CheatCategoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Categories")
	TSubclassOf<UUI_CheatEntry> CheatEntryClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Categories")
	TArray<FCheatCategory> Categories;

	void RequestViewForCategory(const FCheatCategory& InCheatCategory);
	void RequestHomeView(const TArray<FCheatCategory>& InCheatCategories);

	void SynchronizeProperties() override;

	CHEATSHEETUI_API void ConfirmSelection();
	CHEATSHEETUI_API void UpSelection();
	CHEATSHEETUI_API void DownSelection();

	FOnNewCategoryRenderRequested OnNewCategoryRequested;
	FOnNewSelectiom OnNewSelection;

protected:
	void NativeConstruct() override;

private:

	void ApplyEntryNumbers();
	void ApplyNewSelectionIndex();

	TFunction<void(const FCheatCategory&)> RequestCallback;

	TMap<int32, TWeakInterfacePtr<ICheatEntryInterface>> Entries;
	TWeakInterfacePtr<ICheatEntryInterface> CurrentSelection;
	int32 CurrentSelectionIndex;
	int32 TotalEntries;
};
