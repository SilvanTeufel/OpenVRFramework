#include "CybSDKUtils.h"

void CybSDKUtils::SaveObjectFromGarbageCollection(UObject* object)
{
	object->AddToRoot();
}

void CybSDKUtils::FreeObjectForGarbageCollection(UObject* object)
{
	if (object && object->IsValidLowLevel() && !object->HasAnyFlags(RF_BeginDestroyed|RF_FinishDestroyed))
	{
			object->RemoveFromRoot();
	}
}
