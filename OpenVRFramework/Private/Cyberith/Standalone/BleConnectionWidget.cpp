
#include "Cyberith/Standalone/BleConnectionWidget.h"

#include "OpenVRFramework.h"

void UBleConnectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ObjectsHolders.Add(SelectingObjectsHolder);
	ObjectsHolders.Add(ConnectionObjectsHolder);
	ObjectsHolders.Add(FailedObjectsHolder);
	ObjectsHolders.Add(CalibrationObjectsHolder);
	
	NativeConstructDelegate.ExecuteIfBound();
}
