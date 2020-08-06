using UnrealBuildTool;

public class AIBehaviour : ModuleRules
{
	public AIBehaviour(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
			"AIModule",
			"Buildings",
			"GameplayTasks",
            "Engine",
            "CoreUObject"
        });
		
		PublicIncludePaths.AddRange(new string[] {"Runtime/AIBehaviour/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Runtime/AIBehaviour/Private"});
    }
}
