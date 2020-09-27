// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <Runtime/UMG/Public/Components/Border.h>
#include <Runtime/UMG/Public/Components/TextBlock.h>

#include "CheatSheetUI/Public/UI/CheatEntryInterface.h"

#include "UI_CheatEntryBase.generated.h"

UCLASS(hidedropdown, MinimalAPI, abstract)
class UUI_CheatEntryBase : public UUserWidget
	, public ICheatEntryInterface
{
	GENERATED_BODY()

public:

	UUI_CheatEntryBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
		, EntryNumberText(nullptr)
		, EntryNameText(nullptr)
		, SelectionBorder(nullptr)
		, SelectedColour(FLinearColor())
		, EntryNumber(0)
	{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EntryNumberText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EntryNameText;

	UPROPERTY(meta = (BindWidget))
	UBorder* SelectionBorder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selection")
	FLinearColor SelectedColour;

	//ICheatEntryInterface
	virtual void ExecuteEntry() override {};

	virtual void SetEntryNumber(const int32 InNewNumber) override final 
	{ 
		EntryNumber = InNewNumber;
		
		if (EntryNumberText != nullptr)
		{
			EntryNumberText->SetText(FText::FromString(FString::FromInt(EntryNumber)));
		}
	}

	virtual void SetIsSelected(const bool InNewSelection) override final
	{
		if (SelectionBorder != nullptr)
		{
			if (InNewSelection)
			{
				SelectionBorder->SetBrushColor(SelectedColour);
			}
			else
			{
				SelectionBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			}
		}
	}

	virtual const FString GetEntryTip() const override { return FString(); };
	//~ICheatEntryInterface

private:

	int32 EntryNumber;
};
