// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VamGoth : ModuleRules
{
	public VamGoth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"VamGoth",
			"VamGoth/Variant_Platforming",
			"VamGoth/Variant_Platforming/Animation",
			"VamGoth/Variant_Combat",
			"VamGoth/Variant_Combat/AI",
			"VamGoth/Variant_Combat/Animation",
			"VamGoth/Variant_Combat/Gameplay",
			"VamGoth/Variant_Combat/Interfaces",
			"VamGoth/Variant_Combat/UI",
			"VamGoth/Variant_SideScrolling",
			"VamGoth/Variant_SideScrolling/AI",
			"VamGoth/Variant_SideScrolling/Gameplay",
			"VamGoth/Variant_SideScrolling/Interfaces",
			"VamGoth/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
