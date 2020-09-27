// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Engine/Classes/GameFramework/InputSettings.h>

namespace CheatSheetStaticBindings
{
	const FString CheatSheetInputPrefix = TEXT("CHEATSHEET_");

	const FName ShowMenu = TEXT("CHEATSHEET_ShowMenu");
	const FInputActionKeyMapping ShowMenuMapping(ShowMenu, FKey(EKeys::BackSpace), true, false, true, false);

	const FName Confirm = TEXT("CHEATSHEET_Confirm");
	const FInputActionKeyMapping ConfirmMapping(Confirm, FKey(EKeys::Right));

	const FName Up = TEXT("CHEATSHEET_Up");
	const FInputActionKeyMapping UpMapping(Up, FKey(EKeys::Up));

	const FName Down = TEXT("CHEATSHEET_Down");	
	const FInputActionKeyMapping DownMapping(Down, FKey(EKeys::Down));

	const FName Back = TEXT("CHEATSHEET_Back");
	const FInputActionKeyMapping BackMapping(Back, FKey(EKeys::Left));

	void TempPushInputs()
	{
		if (UInputSettings* InputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>()))
		{
			InputSettings->AddActionMapping(ShowMenuMapping);
			InputSettings->AddActionMapping(ConfirmMapping);
			InputSettings->AddActionMapping(UpMapping);
			InputSettings->AddActionMapping(DownMapping);
			InputSettings->AddActionMapping(BackMapping);
		}
	}
}