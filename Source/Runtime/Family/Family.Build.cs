using UnrealBuildTool;

public class Family : ModuleRules
{
	public Family(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/Family/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/Family/Private"});
    }
}
