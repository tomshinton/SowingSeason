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
			"Build",
			"Buildings",
			"Movement",
			"WorldGrid",
			"UI"
		});
	}
}
