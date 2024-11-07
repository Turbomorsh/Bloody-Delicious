// Bloody Delisious. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BloodyDelisiousTarget : TargetRules
{
	public BloodyDelisiousTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "BloodyDelisious" } );
	}
}
