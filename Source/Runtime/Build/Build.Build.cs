using UnrealBuildTool;

public class Build : ModuleRules
{
	public Build(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject",
			"WorldGrid"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/Build/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/Build/Private"});
    }
}
