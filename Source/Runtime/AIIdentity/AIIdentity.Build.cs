using UnrealBuildTool;

public class AIIdentity : ModuleRules
{
	public AIIdentity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/AIIdentity/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/AIIdentity/Private"});
    }
}
