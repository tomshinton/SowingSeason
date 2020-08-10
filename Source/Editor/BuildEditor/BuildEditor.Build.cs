// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;

public class BuildEditor : ModuleRules
{
	public BuildEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Editor/BuildEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Editor/BuildEditor/Private"});
    }
}
