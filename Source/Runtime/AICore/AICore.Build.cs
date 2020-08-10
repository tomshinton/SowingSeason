// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;

public class AICore : ModuleRules
{
	public AICore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
			"AIModule",
			"NavigationSystem",
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/AICore/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/AICore/Private"});
    }
}
