// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectEscape : ModuleRules
{
    public ProjectEscape(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "GameplayTasks", "AnimGraphRuntime", "MotionWarping" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { "ProjectEscape" });

    }
}
