using UnrealBuildTool;

public class Level : ModuleRules
{
	public Level(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject",
			"Landscape",
			"WorldGrid",
        });
		
		PublicIncludePaths.AddRange(new string[] { "Runtime/Level/Public" });
		PrivateIncludePaths.AddRange(new string[] { "Runtime/Level/Private" });
    }
}
