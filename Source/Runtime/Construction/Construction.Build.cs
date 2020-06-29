using UnrealBuildTool;

public class Construction : ModuleRules
{
	public Construction(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
			"Build",
            "Engine",
            "CoreUObject",
			"ObjectMessaging"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/Construction/Public"});
		PrivateIncludePaths.AddRange(new string[] { "Runtime/Construction/Private" });
    }
}
