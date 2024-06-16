package com.cyberith.unrealengineble;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.app.Activity;
import android.content.pm.PackageManager;

import java.nio.charset.StandardCharsets;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.Arrays;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;
import java.util.UUID;
import java.util.ArrayList;

import static android.bluetooth.BluetoothDevice.TRANSPORT_LE;

public class AndroidVirtDevice {
    public static final String TAG = "AndroidVirtDevice";
    protected static final UUID CHARACTERISTIC_UPDATE_NOTIFICATION_DESCRIPTOR_UUID = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    // Virt output service and characteristic UUIDs
    private final UUID VIRT_OUTPUT_UUID = UUID.fromString("235E0559-77C2-4004-B31F-567BCF9DCAF7");
    private final UUID IS_OPEN_UUID = UUID.fromString("D34D8FDB-4C94-49E2-8A12-72744F93C93E");
    private final UUID REPORT_UUID = UUID.fromString("8CD0EADE-DB85-4016-91F0-BD4F71D6F7B1");
    private final UUID HAS_HAPTIC_UUID = UUID.fromString("01A9DCF0-136B-4644-82F7-B6890CDA9AC7");
    // Virt device info service and characteristic UUIDs
    private final UUID VIRT_DEVICE_INFO_UUID = UUID.fromString("DEEBD665-9D32-4EF7-AF4A-F127D9870659");
    private final UUID MAJOR_VERSION_UUID = UUID.fromString("E4AD77CC-25DF-4CAF-802D-0697D5A9688D");
    private final UUID MINOR_VERSION_UUID = UUID.fromString("E7F6943B-10F5-4411-8D4D-81FF693CC0B3");
    private final UUID VENDOR_ID_UUID = UUID.fromString("9F3D359C-771F-437C-8B2B-1FC237A29ACF");
    private final UUID VENDOR_NAME_UUID = UUID.fromString("67AFB3E2-023A-40A9-AD2F-8A0AA1072D9D");
    private final UUID PRODUCT_ID_UUID = UUID.fromString("FADDBB5F-11CC-4EBF-A851-BF020F384B81");
    private final UUID PRODUCT_NAME_UUID = UUID.fromString("5DB09FBE-DF02-4A24-A90A-11479FFA7BA6");
    // Virt input service and characteristic UUIDs
    private final UUID VIRT_INPUT_UUID = UUID.fromString("69B60A0F-6499-4EDD-97AA-B1C3C8514F52");
    private final UUID OPEN_UUID = UUID.fromString("71EE4166-5383-43BE-822D-5A10ECF371B0");
    private final UUID CLOSE_UUID = UUID.fromString("BD6408EF-DE70-41A4-91B6-4F5EF1C7911A");
    private final UUID HAPTIC_PLAY_UUID = UUID.fromString("1B097392-A22C-497F-A3E3-86E5483ED468");
    private final UUID HAPTIC_SET_VALUES_UUID = UUID.fromString("253F1240-1934-45D8-9748-59A652565F3E");
    private final UUID HAPTIC_STOP_UUID = UUID.fromString("C4301F1C-5964-484A-BF91-1EF294420553");
    private final UUID RESET_PLAYER_HEIGHT_UUID = UUID.fromString("3B550CCF-7F19-4DF0-B88A-A9BEFA3FC5B9");
    private final UUID RESET_PLAYER_ORIENTATION_UUID = UUID.fromString("CD75C029-3D0E-402D-9C4C-8F15934D211A");

    public State mState = State.Unknown;
    private BluetoothAdapter mBluetoothAdapter = null;
    private BluetoothGatt mBluetoothGatt = null;
    private Activity mActivity = null;
    private long mCallbackAddr = 0;

    private boolean mPendingOperation = false;

    // Queue that will be used everytime the connection with the virtualizer device is opened
    private final List<BLEOperation> BLE_OPERATION_NEW_CONNECTION_QUEUE = Arrays.asList(
            new BLEOperation(VIRT_OUTPUT_UUID, IS_OPEN_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_OUTPUT_UUID, HAS_HAPTIC_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, MAJOR_VERSION_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, MINOR_VERSION_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, VENDOR_ID_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, VENDOR_NAME_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, PRODUCT_ID_UUID, BLEOperationType.READ),
            new BLEOperation(VIRT_DEVICE_INFO_UUID, PRODUCT_NAME_UUID, BLEOperationType.READ));
    // Queue of all the subscribe operations
    private final List<BLEOperation> BLE_OPERATION_SUBSCRIPTION_QUEUE = Arrays.asList(
            new BLEOperation(VIRT_OUTPUT_UUID, REPORT_UUID, BLEOperationType.SUBSCRIBE),
            new BLEOperation(VIRT_OUTPUT_UUID, IS_OPEN_UUID, BLEOperationType.SUBSCRIBE));
    // Queue that holds all operations
    private Queue<BLEOperation> BLEOperationQueue = new LinkedList<>();
    private Queue<byte[]> WriteDataQueue = new LinkedList<>();

    // Updated values
    private boolean mIsOpen = false;
    private int mPlayerOrientation = 0;
    private int mMovementDirection = 0;
    private int mMovementSpeed = 0;
    private int mPlayerHeight = 0;
    private boolean mHasHaptic = false;
    //Virt device info values
    private int mMajorVersion;
    private int mMinorVersion;
    private int mVendorId;
    private String mVendorName;
    private int mProductId;
    private String mProductName;
    private int mDisconnectCount = 0;
    private int mNullDataCount = 0;
    private String mSavedVirtSamName;

    public enum State {
        Unknown,
        Selecting,
        Connecting,
        Connection_Failed,
        Connected,
        Initializing_Data,
        Active
    }

    //
    // Unreal Engine Callbacks
    //

    public static native void OnStateChange(long callbackAddr, String newState);
    public static native void OnAndroidError(String error);
    public static native void OnAndroidWarning(String warning);

    //
    // BLE
    //
    private BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {

        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                SetState(State.Connected);
                gatt.discoverServices();
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                mPendingOperation = false;
                if (status == 133 || status == 8) {
                    // Either connecting to the virtualizer failed or the connection was lost
                    // Sometimes when connecting, it fails with status 133
                    // But it works to retry once or a few times, otherwise restarting tends to always work
                    if (mDisconnectCount >= 5) {
                        mDisconnectCount = 0;
                        DisconnectAfterFail(status);
                    } else {
                        mDisconnectCount++;
                        mBluetoothGatt = null;
                        mBluetoothGatt = gatt.getDevice().connectGatt(mActivity, false, mGattCallback, TRANSPORT_LE);
                    }
                } else {
                    DisconnectAfterFail(status);
                }
            }
        }

        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                gatt.requestConnectionPriority(BluetoothGatt.CONNECTION_PRIORITY_HIGH);

                SetState(State.Initializing_Data);

                // Populating the operation queue, so the Virtualizer data can be initialized
                BLEOperationQueue.clear();
                BLEOperationQueue.addAll(BLE_OPERATION_NEW_CONNECTION_QUEUE);
                BLEOperationQueue.addAll(BLE_OPERATION_SUBSCRIPTION_QUEUE);

                DoNextOperation();
            } else {
                OnAndroidError("onServicesDiscovered failed with status: " + status);
            }
        }

        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            byte[] data = characteristic.getValue();

            // The first time connecting with the pi after a restart of the pi hard-/software can cause the data to be null
            // Restarting the data initialization process fixes the issue most of the time
            if (data == null) {
                OnAndroidWarning("OnCharacteristicRead: data == null, with status: " + status + ". Restarting 'Initializing Data'. If repeating more than 10 times, restart app.");
                BLEOperationQueue.clear();
                BLEOperationQueue.addAll(BLE_OPERATION_NEW_CONNECTION_QUEUE);
                BLEOperationQueue.addAll(BLE_OPERATION_SUBSCRIPTION_QUEUE);
                mPendingOperation = false;
                
                mNullDataCount += 1;
                if (mNullDataCount >= 10) {
                    mNullDataCount = 0;
                    RestartConnection();
                    return;
                }
                
                DoNextOperation();
                return;
            }

            UUID mUUID = characteristic.getUuid();
            if (mUUID.equals(IS_OPEN_UUID)) {
                mIsOpen = data[0] != 0;
            } else if (mUUID.equals(HAS_HAPTIC_UUID)) {
                mHasHaptic = data[0] != 0;
            } else if (mUUID.equals(MAJOR_VERSION_UUID)) {
                mMajorVersion = DataBytesToInt(data, 0);
            } else if (mUUID.equals(MINOR_VERSION_UUID)) {
                mMinorVersion = DataBytesToInt(data, 0);
            } else if (mUUID.equals(VENDOR_ID_UUID)) {
                mVendorId = DataBytesToInt(data, 0);
            } else if (mUUID.equals(VENDOR_NAME_UUID)) {
                mVendorName = FilterNullCharactersOf(new String(data, StandardCharsets.UTF_16LE));
            } else if (mUUID.equals(PRODUCT_ID_UUID)) {
                mProductId = DataBytesToInt(data, 0);
            } else if (mUUID.equals(PRODUCT_NAME_UUID)) {
                mProductName = FilterNullCharactersOf(new String(data, StandardCharsets.UTF_16LE));
            } else {
                OnAndroidError("OnCharacteristicRead: Did not recognize UUID: " + mUUID);
            }

            mPendingOperation = false;
            DoNextOperation();
        }

        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            byte[] data = characteristic.getValue();

            UUID mUUID = characteristic.getUuid();
            if (mUUID.equals(REPORT_UUID)) {
                mPlayerOrientation = DataBytesToInt(data, 0);
                mPlayerHeight = DataBytesToInt(data, 4);
                mMovementDirection = DataBytesToInt(data, 8);
                mMovementSpeed = DataBytesToInt(data, 12);
            } else if (mUUID.equals(IS_OPEN_UUID)) {
                mIsOpen = data[0] != 0;
            }
        }

        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            mPendingOperation = false;

            // When the list is empty we have reached the end of the initial queue
            if (mState == State.Initializing_Data && BLEOperationQueue.isEmpty()) {
                SetState(State.Active);
            } else {
                DoNextOperation();
            }
        }

        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            mPendingOperation = false;

            switch (status) {
                case BluetoothGatt.GATT_SUCCESS:
                    break;
                case BluetoothGatt.GATT_INVALID_ATTRIBUTE_LENGTH:
                    OnAndroidError("Invalid attribute length write");
                    break;
                case BluetoothGatt.GATT_WRITE_NOT_PERMITTED:
                    OnAndroidError("Write not permitted");
                    break;
                default:
                    OnAndroidError("Write result: unknown");
                    break;
            }

            DoNextOperation();
        }
    };

    private void DisconnectAfterFail(int status) {
        if (mBluetoothGatt != null)
            mBluetoothGatt.close();
        OnAndroidError("Disconnected with status: " + status);
        SetState(State.Connection_Failed);
    }

    private void SetState(State newState) {
        mState = newState;
        OnStateChange(mCallbackAddr, newState.toString());
    }

    private void DoNextOperation() {
        // If no operations are left or operation is still pending, return
        if (BLEOperationQueue.isEmpty() || mPendingOperation) {
            return;
        }

        mPendingOperation = true;

        // Get next operation on the list and remove it from the list
        BLEOperation operation = BLEOperationQueue.poll();

        if (BLEOperationQueue.size() > 50)
            OnAndroidWarning("Warning~Bluetooth operation queue size is more than 50! Increase the haptic period variable in 'VirtDevice_Android.h' to avoid the queue filling up too quickly.");

        if (mBluetoothGatt != null) {
            BluetoothGattService service = mBluetoothGatt.getService(operation.serviceUUID);
            if (service != null) {
                BluetoothGattCharacteristic characteristic = service.getCharacteristic(operation.characteristicUUID);
                if (characteristic != null) {
                    BluetoothGattDescriptor descriptor = characteristic.getDescriptor(CHARACTERISTIC_UPDATE_NOTIFICATION_DESCRIPTOR_UUID);

                    switch (operation.mBLEOperationType) {
                        case READ:
                            mBluetoothGatt.readCharacteristic(characteristic);
                            break;
                        case WRITE:
                            byte[] data = WriteDataQueue.poll();
                            if (data != null) {
                                characteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
                                characteristic.setValue(data);
                                mBluetoothGatt.writeCharacteristic(characteristic);
                            } else {
                                OnAndroidError("Write data is null for characteristic: " + operation.characteristicUUID);
                                mPendingOperation = false;
                            }
                            break;
                        case SUBSCRIBE:
                            if (mBluetoothGatt.setCharacteristicNotification(characteristic, true)) {
                                descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                                if (!mBluetoothGatt.writeDescriptor(descriptor)) {
                                    OnAndroidError("Writing descriptor failed");
                                }
                            } else {
                                OnAndroidError("Subscribe failed");
                            }
                            break;
                        case UNSUBSCRIBE:
                            if (mBluetoothGatt.setCharacteristicNotification(characteristic, false)) {
                                descriptor.setValue(BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE);
                                if (!mBluetoothGatt.writeDescriptor(descriptor)) {
                                    OnAndroidError("Writing descriptor failed");
                                }
                            } else {
                                OnAndroidError("Unsubscribe failed");
                            }
                            break;
                    }
                } else {
                    OnAndroidError("Characteristic not found: " + operation.characteristicUUID);
                    RestartConnection();
                }
            } else {
                OnAndroidError("Service not found: " + operation.serviceUUID);
                RestartConnection();
            }
        } else {
            OnAndroidError("Bluetooth Gatt is null");
            RestartConnection();
        }
    }
    
    private void RestartConnection() {
        OnAndroidWarning("Restarting Connection Process");
        exit();
        findPairedVirtSamAndConnect(mSavedVirtSamName);
    }

    private String FilterNullCharactersOf(String string) {
        String filteredString = "";
        CharacterIterator itr = new StringCharacterIterator(string);
        while (itr.current() != CharacterIterator.DONE) {
            if (itr.current() != '\u0000') {
                filteredString += itr.current();
            }
            itr.next();
        }
        return filteredString;
    }

    private int DataBytesToInt(byte[] data, int offset) {
        int byteCount = data.length;
        int convertedInt = 0;
        switch (byteCount) {
            case 16:
                convertedInt = ((data[offset + 3] & 0xff) << 24)
                        | ((data[offset + 2] & 0xff) << 16)
                        | ((data[offset + 1] & 0xff) << 8)
                        | ((data[offset] & 0xff));
                break;
            case 2:
                convertedInt = ((data[offset + 1] & 0xff) << 8)
                        | ((data[offset] & 0xff));
                break;
            case 1:
                convertedInt = ((data[offset] & 0xff));
                break;
        }
        return convertedInt;
    }

    //****************************************************************************************
    //* Unreal Engine Interface
    //****************************************************************************************

    public AndroidVirtDevice(Activity activity, long callbackAddr) {
        mActivity = activity;
        mCallbackAddr = callbackAddr;
    }

    public void init() {
        if (!mActivity.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            OnAndroidError("No BLE available on this device");
        } else {
            if (mBluetoothAdapter == null) {
                mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            }
        }
    }

    // Go through the list of paired devices to check for a device with the savedName
    // If it has been found connect to it
    // Sends back the result and the list of paired devices
    public String[] findPairedVirtSamAndConnect(String savedName) {
        mSavedVirtSamName = savedName;
        Set<BluetoothDevice> bondedDevices = mBluetoothAdapter.getBondedDevices();
        Iterator<BluetoothDevice> deviceIterator = bondedDevices.iterator();
        ArrayList<String> bondedDevicesNames = new ArrayList<String>();
        bondedDevicesNames.add("false");

        while(deviceIterator.hasNext()) {
            BluetoothDevice device = deviceIterator.next();
            String bondedDeviceName = device.getName();
            bondedDevicesNames.add(bondedDeviceName);

            if (bondedDeviceName.equals(savedName)) {
                mBluetoothGatt = device.connectGatt(mActivity, false, mGattCallback, TRANSPORT_LE);
                SetState(State.Connecting);
                bondedDevicesNames.set(0, "true");
            }
        }

        return bondedDevicesNames.toArray(new String[0]);
    }
    
    public void exit()
    {
        if (mBluetoothGatt != null) {
            mBluetoothGatt.close();
            mBluetoothGatt = null;
        }
    }

    //****************************************************************************************
    //* Virtualizer Device Interface
    //****************************************************************************************
    public boolean open()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, OPEN_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add("Open".getBytes(StandardCharsets.UTF_8));
        DoNextOperation();
        return true;
    }

    public boolean isOpen()
    {
        return mIsOpen;
    }

    public boolean close()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, CLOSE_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add("Close".getBytes(StandardCharsets.UTF_8));
        DoNextOperation();
        return true;
    }

    public int getPlayerHeight()
    {
        return mPlayerHeight;
    }

    public void resetPlayerHeight()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, RESET_PLAYER_HEIGHT_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add("ResetPlayerHeight".getBytes(StandardCharsets.UTF_8));
        DoNextOperation();
    }

    public int getPlayerOrientation()
    {
        return mPlayerOrientation;
    }

    public void resetPlayerOrientation()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, RESET_PLAYER_ORIENTATION_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add("ResetPlayerOrientation".getBytes(StandardCharsets.UTF_8));
        DoNextOperation();
    }

    public int getMovementSpeed()
    {
        return mMovementSpeed;
    }

    public int getMovementDirection()
    {
        return mMovementDirection;
    }

    public int getMajorVersion()
    {
        return mMajorVersion;
    }

    public int getMinorVersion()
    {
        return mMinorVersion;
    }

    public int getVendorId()
    {
        return mVendorId;
    }

    public String getVendorName()
    {
        return mVendorName;
    }

    public int getProductId()
    {
        return mProductId;
    }

    public String getProductName()
    {
        return mProductName;
    }

    public boolean hasHaptic()
    {
        return mHasHaptic;
    }

    public void hapticPlay()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, HAPTIC_PLAY_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add(new byte[] { (byte) 1});
        DoNextOperation();
    }

    public void hapticStop()
    {
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, HAPTIC_STOP_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add(new byte[] { (byte) 1});
        DoNextOperation();
    }

    public void hapticSetValues(int gain, int frequency, int volume)
    {
        byte[] writeData = new byte[] {
                (byte)gain,
                (byte)frequency,
                (byte)volume,
        };
        BLEOperationQueue.add(new BLEOperation(VIRT_INPUT_UUID, HAPTIC_SET_VALUES_UUID, BLEOperationType.WRITE));
        WriteDataQueue.add(writeData);
        DoNextOperation();
    }
}
