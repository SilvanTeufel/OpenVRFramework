#include "CybSDKUtils.h"

void CybSDKUtils::SaveObjectFromGarbageCollection(UObject* object)
{
	object->AddToRoot();
}

void CybSDKUtils::FreeObjectForGarbageCollection(UObject* object)
{
	object->RemoveFromRoot();
}
