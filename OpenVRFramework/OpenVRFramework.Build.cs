// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class OpenVRFramework : ModuleRules
{
	public OpenVRFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AndroidPermission" });
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Core dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "XRBase", "Landscape", "InputDevice" });

		// Gameplay Ability System
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks"});
		
		// Slate dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore",});
		
		// EOS dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "Networking", "Sockets", "OnlineSubsystemEOS", "OnlineSubsystem", "OnlineSubsystemUtils" });
		
		// Ai dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });

		// Hud dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay", "NavigationSystem" });

		// Widget dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "UMG", "MoviePlayer" });
		
		// Cyberith depencies
		//PublicDependencyModuleNames.AddRange(new string[] { "CybSDK", "UnrealEd" });

		// Cyberith depencies
		PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Projects" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { });

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.Add("UnrealEd");
		}
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"OpenVRFramework/Public",
				//"CybSDK_Plugin/Public/Android",
				"OpenVRFramework/Public/Cyberith/Components",
				"OpenVRFramework/Public/Cyberith/CybSDK",
				"OpenVRFramework/Public/Cyberith/Input",
				"OpenVRFramework/Public/Cyberith/Standalone"
				//"ThirdParty/CybSDK",
				// ... add other private include paths required here ...
			}
		);
		
		if (Target.Platform == UnrealTargetPlatform.Win64 /*|| Target.Platform == UnrealTargetPlatform.Win32*/)
		{
			PrivateIncludePaths.Add("ThirdParty/CybSDK");
			// Plugin name
			string PluginName = "OpenVRFramework";

			// Platform (Win32 / Win64)
			string TargetPlatformStr = "Win64";//Target.Platform.ToString();

			// Directory - Input
			string PluginBaseDirectory = Path.Combine(ModuleDirectory, "..", "..");
			// Directory - Output
			string OutputDirectory = Path.Combine("$(TargetOutputDir)", "..", "..", "Plugins", PluginName);

			// CybSDK source (not Unreal-related, Source, Lib, dlls
			string CybSDKSourceDirectory = Path.Combine(PluginBaseDirectory, "Source", "ThirdParty", "CybSDK");
			string CybSDKLibDirectory = Path.Combine(CybSDKSourceDirectory, TargetPlatformStr);
			string CybSDKBinaryPath = Path.Combine(PluginBaseDirectory, "Binaries", "ThirdParty", "CybSDK", TargetPlatformStr);
			string CybSDKDllPath = Path.Combine(CybSDKBinaryPath, "CybSDK.dll");

			// Directories - Output
			string OutputCybSDKBinaryPath = Path.Combine(OutputDirectory, "Binaries", "ThirdParty", "CybSDK", TargetPlatformStr);
			string OutputCybSDKDllPath = Path.Combine(OutputCybSDKBinaryPath, "CybSDK.dll");

			// Include path to the .h
			PublicIncludePaths.Add(CybSDKSourceDirectory);
			PublicSystemIncludePaths.Add(CybSDKSourceDirectory);

			// Lib
// #if UE_4_21_OR_LATER
			PublicAdditionalLibraries.Add(Path.Combine(CybSDKLibDirectory, "CybSDK.lib"));
// #else
//			PublicLibraryPaths.Add(CybSDKLibDirectory); 	// in 4.25, 4.26 and later, we should use PublicAdditionalLibraries instead
// #endif

			// DLL - Load
			PublicDelayLoadDLLs.Add( "CybSDK.dll");
			
			// DLL Auto-copying
// #if UE_4_21_OR_LATER
			RuntimeDependencies.Add(Path.Combine(CybSDKBinaryPath, "CybSDK.dll"));
			//RuntimeDependencies.Add(Path.Combine(CybSDKBinaryPath, "CybSDK.dll"));
// #else
//			RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(CybSDKBinaryPath, "CybSDK.dll")));	// in 4.25, 4.26 and later, we should use PublicAdditionalLibraries instead
// #endif
		}
	}
}
