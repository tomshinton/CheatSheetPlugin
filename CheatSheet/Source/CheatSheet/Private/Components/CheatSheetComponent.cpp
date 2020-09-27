// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheet/Public/Components/CheatSheetComponent.h"
#include "CheatSheet/Public/CheatSheet.h"

UCheatSheetComponent::UCheatSheetComponent()
{
}

void UCheatSheetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* OwningController = Cast<APlayerController>(GetOwner()))
	{
		FCheatSheetModule::Init(*OwningController->GetPawn());
	}
}

void UCheatSheetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		FCheatSheetModule::Cleanup();
	}

	Super::EndPlay(EndPlayReason);
}
