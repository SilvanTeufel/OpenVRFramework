package com.cyberith.unrealengineble;

public class CalibrationData {
    int result;
    float calibratedPositionX;
    float calibratedPositionZ;
    float calibratedRotationY;
    float boundary0X;
    float boundary0Z;
    float boundary1X;
    float boundary1Z;

    public CalibrationData(int result,float calibratedPositionX,
            float calibratedPositionZ,
            float calibratedRotationY,
            float boundary0X,
            float boundary0Z,
            float boundary1X,
            float boundary1Z){
        this.result = result;
        this.calibratedPositionX = calibratedPositionX;
        this.calibratedPositionZ = calibratedPositionZ;
        this.calibratedRotationY = calibratedRotationY;
        this.boundary0X = boundary0X;
        this.boundary0Z = boundary0Z;
        this.boundary1X = boundary1X;
        this.boundary1Z = boundary1Z;
    }

    public CalibrationData(int error) {
        this.result = error;
    }
}
