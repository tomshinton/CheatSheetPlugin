// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/Entries/UI_CheatEntry.h"

DEFINE_LOG_CATEGORY_STATIC(CheatEntryLog, Log, Log);

UUI_CheatEntry::UUI_CheatEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Cheat(FCachedCheat())
{

}

void UUI_CheatEntry::ExecuteEntry()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* LocalPlayerController = World->GetFirstPlayerController<APlayerController>())
		{
			LocalPlayerController->ConsoleCommand(Cheat.CheatString);
			OnCheatEntryExecuted.Execute(Cheat);
		}
	}
}

const FString UUI_CheatEntry::GetEntryTip() const
{
	return Cheat.Tooltip;
}

void UUI_CheatEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (EntryNameText != nullptr)
	{
		if(!Cheat.DisplayName.IsEmpty())
		{
			EntryNameText->SetText(FText::FromString(Cheat.DisplayName));
		}
		else
		{
			EntryNameText->SetColorAndOpacity(FSlateColor(FColorList::Scarlet));
			EntryNameText->SetText(FText::FromString(Cheat.CheatString));
		}
	}
}
