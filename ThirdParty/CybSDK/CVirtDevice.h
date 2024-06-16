/************************************************************************************
Filename    :   CVirtDevice.h
Content     :   class VirtDevice is the main interface to communicate to Virtualizer devices.
				To add your own custom implementation you should derive from this base class.

Copyright 2019 Cyberith GmbH
Authors		:	Lukas Pfeifhofer, Jeroen Doggen, Stefan Radlwimmer <s.radlwimmer@cyberith.com>
Support		:	support@cyberith.com
*************************************************************************************/

#ifndef CVirtDevice_h
#define CVirtDevice_h

#include "CVirtExport.h"
#include "CVirtDeviceInfo.h"

namespace CybSDK
{
	/**
	 * @brief Main interface all Virtualizer Devices have to implement.
	 */
	class CYBSDK_EXPORT VirtDevice
	{
	public:
		//****************************************************************************************
		//* Ctor/Dtor
		//****************************************************************************************

		virtual ~VirtDevice() = default;


		//****************************************************************************************
		//* Connection
		//****************************************************************************************

		/**
		 * @brief Opens the connection to the Virtualizer device.
		 * @remark No other application can open this device at the same time.
		 * @return true if the connection was successfully opened, otherwise false.
		 */
		virtual bool Open() = 0;
		/**
		 * @brief Checks if the connection was opened before.
		 */
		virtual bool IsOpen() const = 0;
		/**
		 * @brief Closes the connection to the Virtualizer device.
		 */
		virtual bool Close() = 0;


		//****************************************************************************************
		//* Device Infos
		//****************************************************************************************

		/**
		 * @brief Returns the USB infos of this device.
		 */
		virtual const VirtDeviceInfo& GetDeviceInfo() const = 0;

		//****************************************************************************************
		//* Virtualizer Game Data
		//****************************************************************************************

		/**
		 * @brief Returns the current player height relative to the default height.
		 * @remark The default height is set by the ResetPlayerHeight method.
		 * @remark height < -threshold: crouching
		 * @remark height >  threshold: jumping
		 * @return 1.00f = 1cm.
		 */
		virtual float GetPlayerHeight() const = 0;

		/**
		 * @brief Assigns the current height to the default height.
		 * @remark This method should be called while the player is asked to stand upright.
		 */
		virtual void ResetPlayerHeight() = 0;

		/**
		 * @brief Returns the orientation of the player as an absolute value.
		 * @remark The origin is set by the ResetPlayerOrientation method and increases clockwise.
		 * @return logical: 0.00f to 1.00f (= physical: 0° to 360°).
		 */
		virtual float GetPlayerOrientation() const = 0;

		/**
		 * @brief Assigns the current orientation to the default vector.
		 * @remark This method should be called while the player is asked to look forward.
		 * @remark This orientation should be used to calibrate the HMD.
		 * @deprecated Unused as new Virtualizers orient themselves automatically.
		 */
		virtual void ResetPlayerOrientation() = 0;

		/**
		 * @brief Returns the current movement speed in meters per second.
		 * @return 1.00f = 1m/s
		 */
		virtual float GetMovementSpeed() const = 0;

		/**
		 * @brief Returns the movement direction relative to the current player orientation.
		 * @remark The origin is the GetPlayerOrientation method and increases clockwise.
		 * @return logical: -1.00f to 1.00f (= physical: -180° to 180°).
		 */
		virtual float GetMovementDirection() const = 0;

		//****************************************************************************************
		//* Haptic
		//****************************************************************************************

		/**
		 * @brief Checks if the Virtualizer device supports haptic feedback.
		 */
		virtual bool HasHaptic() const = 0;

		/**
		 * @brief Play a signal on the haptic unit.
		 */
		virtual void HapticPlay() = 0;

		/**
		 * @brief Stop the haptic unit.
		 */
		virtual void HapticStop() = 0;

		/**
		 * @brief Set the gain (dB) level of the haptic unit.
		 * @param gain The value can be 0, 1, 2 or 3.
		 */
		virtual void HapticSetGain(int gain) = 0;

		/**
		 * @brief Set the frequency (Hz) of a sine wave on the haptic unit.
		 * @param frequency The value is valid between 10Hz and 80Hz.
		 */
		virtual void HapticSetFrequency(int frequency) = 0;

		/**
		 * @brief Sets the haptic feedback (change of amplitude) in the baseplate.
		 * @param volume The value is valid between 0 (no feedback) and 100 (full feedback).
		 */
		virtual void HapticSetVolume(int volume) = 0;
	};
}
#endif