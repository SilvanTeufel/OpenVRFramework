package com.cyberith.unrealengineble;

import java.util.UUID;

public class BLEOperation {
    public final UUID serviceUUID;
    public final UUID characteristicUUID;
    public final BLEOperationType mBLEOperationType;

    public BLEOperation(UUID serviceUUID, UUID characteristicUUID, BLEOperationType mBLEOperationType) {
        this.serviceUUID = serviceUUID;
        this.characteristicUUID = characteristicUUID;
        this.mBLEOperationType = mBLEOperationType;
    }
}
