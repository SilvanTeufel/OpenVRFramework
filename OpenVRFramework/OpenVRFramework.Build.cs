// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class OpenVRFramework : ModuleRules
{
	public OpenVRFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[] { "AndroidPermission" });
		
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
		
#if UE_5_1_OR_LATER
        // Cyberith dependencies for UE 5.1 or later
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "ApplicationCore",
        });
#endif

        // Projects
        PublicDependencyModuleNames.AddRange(new string[]
        {
                "Projects"
        });
        PrivateDependencyModuleNames.AddRange(new string[]
        {
                "Projects"
        });

        // Editor only dependencies
        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",
                "Projects"
            });
        }

        PrivateIncludePaths.AddRange(
            new string[] {
                "OpenVRFramework/Public",
                "OpenVRFramework/Public/Cyberith/Components",
                "OpenVRFramework/Public/Cyberith/CybSDK",
                "OpenVRFramework/Public/Cyberith/Input",
                "OpenVRFramework/Public/Cyberith/Standalone"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateIncludePaths.Add("ThirdParty/CybSDK");

            // Plugin name
            string PluginName = "OpenVRFramework";

            // Platform (Win64)
            string TargetPlatformStr = "Win64";

            // Directory - Input
            string PluginBaseDirectory = Path.Combine(ModuleDirectory, "..", "..");

            // CybSDK source (not Unreal-related, Source, Lib, dlls)
            string CybSDKSourceDirectory = Path.Combine(PluginBaseDirectory, "Source", "ThirdParty", "CybSDK");
            string CybSDKLibDirectory = Path.Combine(CybSDKSourceDirectory, TargetPlatformStr);
            string CybSDKBinaryPath = Path.Combine(PluginBaseDirectory, "Binaries", "ThirdParty", "CybSDK", TargetPlatformStr);

            // Include path to the .h
            PublicIncludePaths.Add(CybSDKSourceDirectory);
            PublicSystemIncludePaths.Add(CybSDKSourceDirectory);

            // Lib
            PublicAdditionalLibraries.Add(Path.Combine(CybSDKLibDirectory, "CybSDK.lib"));

            // DLL - Load
            PublicDelayLoadDLLs.Add("CybSDK.dll");

            // DLL Auto-copying
            RuntimeDependencies.Add(Path.Combine(CybSDKBinaryPath, "CybSDK.dll"));
		}
	}
}
