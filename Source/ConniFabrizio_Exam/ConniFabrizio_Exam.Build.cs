// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ConniFabrizio_Exam : ModuleRules
{
	public ConniFabrizio_Exam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
