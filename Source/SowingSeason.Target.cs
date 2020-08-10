// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class SowingSeasonTarget : TargetRules
{
	public SowingSeasonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "SowingSeason" } );
	}
}
