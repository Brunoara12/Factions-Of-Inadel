// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FactionsOfInadel : ModuleRules
{
	public FactionsOfInadel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem" });
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "UMG", "Slate", "SlateCore"});
	}
}
