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
"Inventory",
			"AICore",
			"Build",
			"Buildings",
			"Construction",
			"Movement",
			"Level",
			"ObjectMessaging",
			"Selection",
			"WorldGrid",
			"UI"
		});
	}
}
