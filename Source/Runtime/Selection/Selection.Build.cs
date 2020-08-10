// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;

public class Selection : ModuleRules
{
	public Selection(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] { "Runtime/Selection/Public" });
		PrivateIncludePaths.AddRange(new string[] { "Runtime/Selection/Private" });
    }
}
