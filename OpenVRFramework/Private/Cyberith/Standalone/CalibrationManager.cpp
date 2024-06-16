#include "Cyberith/Standalone/CalibrationManager.h"

#include "OpenVRFramework.h"
#include "VRNotificationsComponent.h"
#include "GameFramework/Actor.h"
#include "VectorTypes.h"
#include "Cyberith/Standalone/StandaloneVirtDeviceInterface.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"
#include "Android/Utils/JavaTypesConvertor.h"
#endif

// Sets default values for this component's properties
UCalibrationManager::UCalibrationManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	// ...
}

ECalibrationDataResult UCalibrationManager::GetCalibrationDataResult() const
{
	return m_calibrationDataResult;
}

// Called when the game starts
void UCalibrationManager::BeginPlay()
{
	Super::BeginPlay();
	// Find scene component of the parent actor using the variable name "VROrigin"
	for (UActorComponent* Component : GetOwner()->GetComponents()) // Iterate through the components of the parent Actor
	{
		if (Component && Component->GetName() == "VROrigin")
		{
			m_VrOriginComponent = dynamic_cast<USceneComponent*>(Component);
			if (m_VrOriginComponent == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] CalibrationManager : VROrigin Dynamic cast failed"));
			}
		}
	}
	if (m_VrOriginComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] CalibrationManager : VROrigin component not found"));
	}

	LoadCalibrationData();

	if (m_calibrationDataResult == Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] CalibrationManager : Calibration data loaded"));
		UVRNotificationsComponent* VRNotificationsComponent = NewObject<UVRNotificationsComponent>(GetOuter(), UVRNotificationsComponent::StaticClass(), FName("VRNotifications"));
		VRNotificationsComponent->RegisterComponent();
		VRNotificationsComponent->VRControllerRecenteredDelegate.AddDynamic(this, &UCalibrationManager::OnRecentered);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CYBERITH] CalibrationManager : No calibration data loaded"));
	}
}

void UCalibrationManager::OnRegister()
{
	Super::OnRegister();
	PrimaryComponentTick.bCanEverTick = true;
}

void UCalibrationManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (FOpenVRFrameworkModule::GetVirtualizerInputDevice() != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] CalibrationManager : Initial calibration"));
		SetComponentTickEnabled(false);
		OnRecentered();
	}
}

void UCalibrationManager::CalculateTransform(float boundaryPoints[2][2])
{
	//UE_LOG(LogTemp, Display, TEXT("[CYBERITH] Boundary points: %f, %f, %f, %f"), boundaryPoints[0][0], boundaryPoints[0][1], boundaryPoints[1][0], boundaryPoints[1][1]);
	const hMatrix H = GetHomogeneousTransformationMatrix(boundaryPoints);
	
	/* Offset VR Origin location using the new Transform matrix */
	FVector newRelLocation = FVector(
		-H.m[0][2] - H.m[0][0] * m_calibratedPositionX - H.m[0][1] * m_calibratedPositionY,
		-H.m[1][2] - H.m[1][0] * m_calibratedPositionX - H.m[1][1] * m_calibratedPositionY,
		m_VrOriginComponent->GetRelativeLocation().Z
		);

	m_VrOriginComponent->SetRelativeLocation(newRelLocation);

	float rotationOffset = 0.0f;
	// Switched minus/plus signs to get the correct rotation in UE
	if (FMath::Asin(H.m[1][0]) < 0)
	{
		rotationOffset = m_calibratedRotationZ + (FMath::Acos(H.m[0][0]) * 180.0f / PI);
	}
	else
	{
		rotationOffset = m_calibratedRotationZ - (FMath::Acos(H.m[0][0]) * 180.0f / PI);
	}
	
	// Change orientation offset in VirtDevice_Android
	TScriptInterface<IStandaloneVirtDeviceInterface> standaloneVirtDeviceInterface = FOpenVRFrameworkModule::GetVirtualizerInputDevice()->GetDevice()->GetStandaloneVirtDeviceInterface();
	standaloneVirtDeviceInterface->SetOrientationOffset(rotationOffset);
}

void UCalibrationManager::LoadCalibrationData()
{
	ECalibrationDataResult result = ECalibrationDataResult::Exception;
#if PLATFORM_ANDROID
	TSharedPtr<FJavaClassObject> m_javaClassObject = MakeShared<FJavaClassObject>("com/cyberith/unrealengineble/CalibrationDataManager", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis);

	FJavaClassMethod m_getCalibrationDataMethod = m_javaClassObject->GetClassMethod("getCalibrationData", "()Lcom/cyberith/unrealengineble/CalibrationData;");
	
	jobject JavaCalibrationData = m_javaClassObject->CallMethod<jobject>(m_getCalibrationDataMethod);

	JNIEnv* env = FAndroidApplication::GetJavaEnv();

	jclass JavaCalibrationDataClass = env->GetObjectClass(JavaCalibrationData);

	jfieldID fidResult = env->GetFieldID(JavaCalibrationDataClass, "result", "I");
	jint resultInt = env->GetIntField(JavaCalibrationData, fidResult);
	
	result = static_cast<ECalibrationDataResult>(resultInt);

	if (result == Success)
	{
		// Get the calibration data from Android and convert it to UE units
		// The calibration data is in cm, but UE uses meters
		// For the coordinate system, the X-axis in Unity, is the Y-axis in UE
		// For the coordinate system, the Y-axis in Unity, is the Z-axis in UE
		// For the coordinate system, the Z-axis in Unity, is the X-axis in UE
		jfieldID fidCalibratedPositionX = env->GetFieldID(JavaCalibrationDataClass, "calibratedPositionZ", "F");
		m_calibratedPositionX = env->GetFloatField(JavaCalibrationData, fidCalibratedPositionX) * 100.0f;
		jfieldID fidCalibratedPositionY = env->GetFieldID(JavaCalibrationDataClass, "calibratedPositionX", "F");
		m_calibratedPositionY = env->GetFloatField(JavaCalibrationData, fidCalibratedPositionY) * 100.0f;
		jfieldID fidCalibratedRotationZ = env->GetFieldID(JavaCalibrationDataClass, "calibratedRotationY", "F");
		m_calibratedRotationZ = env->GetFloatField(JavaCalibrationData, fidCalibratedRotationZ);
		jfieldID fidBoundary0X = env->GetFieldID(JavaCalibrationDataClass, "boundary0Z", "F");
		m_boundary0X = env->GetFloatField(JavaCalibrationData, fidBoundary0X) * 100.0f;
		jfieldID fidBoundary0Y = env->GetFieldID(JavaCalibrationDataClass, "boundary0X", "F");
		m_boundary0Y = env->GetFloatField(JavaCalibrationData, fidBoundary0Y) * 100.0f;
		jfieldID fidBoundary1X = env->GetFieldID(JavaCalibrationDataClass, "boundary1Z", "F");
		m_boundary1X = env->GetFloatField(JavaCalibrationData, fidBoundary1X) * 100.0f;
		jfieldID fidBoundary1Y = env->GetFieldID(JavaCalibrationDataClass, "boundary1X", "F");
		m_boundary1Y = env->GetFloatField(JavaCalibrationData, fidBoundary1Y) * 100.0f;

		UE_LOG(LogTemp, Display, TEXT("[CYBERITH] CalibrationManager : Data read: %f, %f, %f, %f, %f, %f, %f "), m_calibratedPositionX, m_calibratedPositionY, m_calibratedRotationZ, m_boundary0X, m_boundary0Y, m_boundary1X, m_boundary1Y);
	}
	else
	{
		// Show error reading calibration data with result
		UE_LOG(LogTemp, Error, TEXT("[CYBERITH] CalibrationManager : Error reading calibration data %d"), resultInt)
		
	}
#endif

	m_calibrationDataResult = result;
}


UCalibrationManager::hMatrix UCalibrationManager::GetHomogeneousTransformationMatrix(float boundaryPoints[2][2]) const
{
	const rMatrix R = GetRotationMatrix(boundaryPoints);
	const tVector t = GetTranslationVector(boundaryPoints, R);

	// H =  [ R    t  ]
	//          [ 0    1 ]
	return { { 
		{ R.m[0][0], R.m[0][1], t.v[0] },
		{ R.m[1][0], R.m[1][1], t.v[1]} 
		} };
}

UCalibrationManager::rMatrix UCalibrationManager::GetRotationMatrix(float boundaryPoints[2][2]) const
{
	// Vector boundary 0 to boundary 1
	FVector2f calibratedBoundary0_1 = FVector2f(m_boundary1X - m_boundary0X, m_boundary1Y - m_boundary0Y);
	// Create unit vector with x1 and y1
	calibratedBoundary0_1.Normalize();

	FVector2f boundary0_1 = FVector2f(boundaryPoints[1][0] - boundaryPoints[0][0], boundaryPoints[1][1] - boundaryPoints[0][1]);
	boundary0_1.Normalize();

	// R	 =		[ x1x2 + y1y2      - (x1y2 - x2y1) ]
	//				[ x1y2 - x2y1            x1x2 + y1y2 ]
	return { {
		{ (calibratedBoundary0_1.X * boundary0_1.X) + (calibratedBoundary0_1.Y * boundary0_1.Y), -1 * ((calibratedBoundary0_1.X * boundary0_1.Y) - (boundary0_1.X * calibratedBoundary0_1.Y)) } ,
		{ (calibratedBoundary0_1.X * boundary0_1.Y) - (boundary0_1.X * calibratedBoundary0_1.Y), (calibratedBoundary0_1.X * boundary0_1.X) + (calibratedBoundary0_1.Y * boundary0_1.Y) }
} };
}

UCalibrationManager::tVector UCalibrationManager::GetTranslationVector(float boundaryPoints[2][2], rMatrix R) const
{
	// t = p2 - (R * p1)
	return { {
			boundaryPoints[0][0] - (R.m[0][0] * m_boundary0X + R.m[0][1] * m_boundary0Y),
			boundaryPoints[0][1] - (R.m[1][0] * m_boundary0X + R.m[1][1] * m_boundary0Y)
			} };
}
