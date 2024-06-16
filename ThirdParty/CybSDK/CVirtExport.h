/************************************************************************************
Filename    :   CVirtExport.h
Content     :   Macros for building and importing CybSDK.dll.
				define BUILD_CYBSDK_STATIC when building the static 'CybSDK_static.lib'
				define CYBSDK_STATIC when using/linking the static 'CybSDK_static.lib'
				define BUILD_CYBSDK_DLL when building the dynamic 'CybSDK.dll'
				define nothing when using/linking the dynamic 'CybSDK.dll'

Copyright 2019 Cyberith GmbH
Authors		:	Lukas Pfeifhofer, Jeroen Doggen, Stefan Radlwimmer <s.radlwimmer@cyberith.com>
Support		:	support@cyberith.com
*************************************************************************************/

#ifndef CVirt_Export_h
#define CVirt_Export_h

#if defined(BUILD_CYBSDK_STATIC) || defined(CYBSDK_STATIC)
#define CYBSDK_EXPORT 
#else
#ifdef BUILD_CYBSDK_DLL
#define CYBSDK_EXPORT __declspec(dllexport)
#else
#define CYBSDK_EXPORT __declspec(dllimport)
#endif
#endif

#endif