#pragma once

class CybSDKUtils
{
public:
	static void SaveObjectFromGarbageCollection(UObject* object);

	static void FreeObjectForGarbageCollection(UObject* object);
};
