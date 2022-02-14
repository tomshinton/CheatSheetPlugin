// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Runtime/Engine/Classes/GameFramework/InputSettings.h>

namespace CheatSheetStaticBindings
{
	const FName ShowMenu = TEXT("CHEATSHEET_ShowMenu");
	const FInputActionKeyMapping ShowMenuMapping(ShowMenu, FKey(EKeys::BackSpace), true, false, true, false);

	const FName Confirm = TEXT("CHEATSHEET_Confirm");
	const FInputActionKeyMapping ConfirmMapping(Confirm, FKey(EKeys::Enter));

	void TempPushInputs()
	{
		if (UInputSettings* InputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>()))
		{
			InputSettings->AddActionMapping(ShowMenuMapping);
			InputSettings->AddActionMapping(ConfirmMapping);
		}
	}
}