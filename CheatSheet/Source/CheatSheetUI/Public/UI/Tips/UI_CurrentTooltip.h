// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "UI_CurrentTooltip.generated.h"

class UTextBlock;

UCLASS(hidedropdown, MinimalAPI)
class UUI_CurrentTooltip: public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_CurrentTooltip(const FObjectInitializer& ObjectInitializer);

	void SetTooltip(const FString& InNewTooltip);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip")
	FString CurrentTooltip;

private:

	void SynchronizeProperties();
};
