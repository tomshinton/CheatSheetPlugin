// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <Runtime/Engine/Classes/GameFramework/PlayerInput.h>

#include "UI_ControlTip.generated.h"

class UTextBlock;

UCLASS(hidedropdown, MinimalAPI)
class UUI_ControlTip : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_ControlTip(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Action;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Keys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Categories")
	FInputActionKeyMapping Mapping;

	void SynchronizeProperties() override;

private:

	const FString GetChordAsString() const; 
};
