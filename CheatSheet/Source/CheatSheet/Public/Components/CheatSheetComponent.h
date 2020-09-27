// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "CheatSheetComponent.generated.h"

UCLASS(Within=PlayerController, MinimalAPI)
class UCheatSheetComponent : public UActorComponent
{
	GENERATED_BODY()

	UCheatSheetComponent();

public:

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};