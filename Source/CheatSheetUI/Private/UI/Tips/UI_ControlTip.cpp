// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/UI/Tips/UI_ControlTip.h"

#include <Runtime/UMG/Public/Components/TextBlock.h>

UUI_ControlTip::UUI_ControlTip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Action(nullptr)
	, Keys(nullptr)
	, Mapping()
{

}

void UUI_ControlTip::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (Action != nullptr)
	{
		Action->SetText(FText::FromString(*Mapping.ActionName.ToString()));
	}

	if (Keys != nullptr)
	{
		Keys->SetText(FText::FromString(GetChordAsString()));
	}
}

const FString UUI_ControlTip::GetChordAsString() const
{
	FString Chord;

	Chord = Mapping.Key.ToString();

	if (Mapping.bAlt)
	{
		Chord += TEXT(" + Alt");
	}

	if (Mapping.bCmd)
	{
		Chord += TEXT(" + Cmd");
	}

	if (Mapping.bCtrl)
	{
		Chord += TEXT(" + Ctrl");
	}

	if (Mapping.bShift)
	{
		Chord += TEXT(" + Shift");
	}

	return Chord;
}
