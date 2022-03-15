// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CheatSheetEditor : ModuleRules
{
    public CheatSheetEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "CheatSheet",
            "CheatSheetTypes",
            "DetailCustomizations",
            "Engine",
            "Projects",
            "Slate",
            "SlateCore",
            "UnrealEd",
            "PropertyEditor"
        });
    }
}
