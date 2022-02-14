// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/Entries/UI_CategoryEntry.h"

#include <Runtime/UMG/Public/Components/TextBlock.h>

UUI_CategoryEntry::UUI_CategoryEntry(const FObjectInitializer& ObjectInitializer)
	: UUI_CheatEntryBase(ObjectInitializer)
	, NumSubCategoriesText(nullptr)
	, Category(FCheatCategory())
	, RequestCallback(nullptr)
{}

void UUI_CategoryEntry::ExecuteEntry()
{
	if (RequestCallback != nullptr)
	{
		RequestCallback(Category);
	}
}

void UUI_CategoryEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (EntryNameText != nullptr)
	{
		EntryNameText->SetText(FText::FromString(Category.GetCategoryName()));
	}

	if (NumSubCategoriesText)
	{
		const FString SubCategoryTest = FString::Printf(TEXT("(%i)"), Category.GetSubCategories().Num() + Category.GetCheats().Num());
		NumSubCategoriesText->SetText(FText::FromString(SubCategoryTest));
	}
}
