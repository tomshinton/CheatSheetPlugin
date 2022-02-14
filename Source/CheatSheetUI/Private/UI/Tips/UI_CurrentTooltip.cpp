// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/Tips/UI_CurrentTooltip.h"

UUI_CurrentTooltip::UUI_CurrentTooltip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Tooltip(nullptr)
	, CurrentTooltip(TEXT(""))
{}

void UUI_CurrentTooltip::SetTooltip(const FString& InNewTooltip)
{
	if (InNewTooltip.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);

		if (Tooltip != nullptr)
		{
			Tooltip->SetText(FText::FromString(*InNewTooltip));
		}
	}
}

void UUI_CurrentTooltip::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (Tooltip != nullptr)
	{
		Tooltip->SetText(FText::FromString(*CurrentTooltip));
	}
}
