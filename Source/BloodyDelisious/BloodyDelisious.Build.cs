// Bloody Delisious. All Rights Reserved.

using UnrealBuildTool;

public class BloodyDelisious : ModuleRules
{
    public BloodyDelisious(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "EnhancedInput",
            "GameplayTasks",
            "NavigationSystem",
            "Paper2D"
        });
        PublicIncludePaths.AddRange(new string[] { "BloodyDelisious" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
