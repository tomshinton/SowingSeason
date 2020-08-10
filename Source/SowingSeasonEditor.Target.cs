// Sowing Season - Tom Shinton 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class SowingSeasonEditorTarget : TargetRules
{
	public SowingSeasonEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] 
		{ 
			"SowingSeason",
			"BuildEditor"
		} );
	}
}
