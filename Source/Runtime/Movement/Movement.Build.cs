using UnrealBuildTool;

public class Movement : ModuleRules
{
	public Movement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/Movement/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/Movement/Private"});
    }
}
