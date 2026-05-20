// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KingOfTheHill : ModuleRules
{
	public KingOfTheHill(ReadOnlyTargetRules Target) : base(Target)
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
			"KingOfTheHill",
			"KingOfTheHill/Variant_Horror",
			"KingOfTheHill/Variant_Horror/UI",
			"KingOfTheHill/Variant_Shooter",
			"KingOfTheHill/Variant_Shooter/AI",
			"KingOfTheHill/Variant_Shooter/UI",
			"KingOfTheHill/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
