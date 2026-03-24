// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Nara : ModuleRules
{
	public Nara(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Nara"
			}
		);

	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameFeatures",
			"CommonLoadingScreen",
			"ControlFlows",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CommonUI",
			"CommonGame",
			"CommonUser",
			"UIExtension",
			"Slate",
			"SlateCore",
			"UMG", 
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
