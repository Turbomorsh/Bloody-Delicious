// Bloody Delisious. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BloodyDelisiousEditorTarget : TargetRules
{
	public BloodyDelisiousEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "BloodyDelisious" } );
	}
}
