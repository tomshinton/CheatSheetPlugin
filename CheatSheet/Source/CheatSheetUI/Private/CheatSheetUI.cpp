// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheetUI/Public/CheatSheetUI.h"

DEFINE_LOG_CATEGORY_STATIC(CheatSheetUILog, Log, Log);

void FCheatSheetUIModule::StartupModule()
{
	UE_LOG(CheatSheetUILog, Log, TEXT("Spinning up CheatSheetUI"));
}

void FCheatSheetUIModule::ShutdownModule()
{
	UE_LOG(CheatSheetUILog, Log, TEXT("Shutting down CheatSheetUI"));
}

IMPLEMENT_MODULE(FCheatSheetUIModule, CheatSheetUI)