// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/UI_CheatCategory.h"

#include <CheatSheetTypes/Public/Categories/CheatCategory.h>

#include <Runtime/UMG/Public/Components/TextBlock.h>

UUI_CheatCategory::UUI_CheatCategory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CheatCategoryNumber(nullptr)
	, CheatCategoryName(nullptr)
	, Category(FCheatCategory())
{}

void UUI_CheatCategory::NativeConstruct()
{
	Super::NativeConstruct();

	if (CheatCategoryName != nullptr)
	{
		CheatCategoryName->SetText(FText::FromString(Category.GetCategoryName()));
	}
}
