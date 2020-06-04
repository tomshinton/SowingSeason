using UnrealBuildTool;

public class Buildings : ModuleRules
{
	public Buildings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/Buildings/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/Buildings/Private"});
    }
}
