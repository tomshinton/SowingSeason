// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;

public class UI : ModuleRules
{
	public UI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject", 
			"Selection",
			"Inventory"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/UI/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/UI/Private"});
    }
}
