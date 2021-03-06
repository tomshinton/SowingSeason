// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;

public class SowingSeason : ModuleRules
{
	public SowingSeason(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore"
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AIBehaviour",
            "AICore",
			"AIIdentity",
			"Build",
			"Buildings",
			"Construction",
            "Inventory",
			"Movement",
			"Level",
			"ObjectMessaging",
			"Selection",
			"WorldGrid",
			"UI"
		});
	}
}
