// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CheatSheetEditor : ModuleRules
{
    public CheatSheetEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
        new string[]
        {
            "Core",
        }
        );


        PrivateDependencyModuleNames.AddRange(
        new string[]
        {
            "CoreUObject",
            "CheatSheetTypes",
            "Engine",
            "Projects",
            "SlateCore",
            "UnrealEd",
		}
        );
    }
}
