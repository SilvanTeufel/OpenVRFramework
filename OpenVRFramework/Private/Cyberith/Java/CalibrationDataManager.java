package com.cyberith.unrealengineble;

import android.app.Activity;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

public class CalibrationDataManager {
    static final String PROVIDER_NAME = "com.cyberith.provider.CyberithCalibrationDataProvider";
    static final String PATH_DATA = "data";
    // defining content URI
    static final String URL = "content://" + PROVIDER_NAME + "/" + PATH_DATA;
    // parsing the content URI
    static final Uri CONTENT_URI = Uri.parse(URL);

    public static final String CALIBRATED_POSITION_X = "calibratedPositionX";
    public static final String CALIBRATED_POSITION_Z = "calibratedPositionZ";
    public static final String CALIBRATED_ROTATION_Y = "calibratedRotationY";
    public static final String BOUNDARY_0_X = "boundary0X";
    public static final String BOUNDARY_0_Z = "boundary0Z";
    public static final String BOUNDARY_1_X = "boundary1X";
    public static final String BOUNDARY_1_Z = "boundary1Z";

    private Activity mActivity;

    public CalibrationDataManager(Activity activity) {
        mActivity = activity;
    }

    public CalibrationData getCalibrationData() {
        Cursor cursor;
        try {
            Context context = mActivity.getApplicationContext();
            cursor = context.getContentResolver().query(CONTENT_URI, null, null, null, null);
        } catch (Exception e) {
            return new CalibrationData(0); // Exception when getting cursor
        }

        if (cursor == null)
            return new CalibrationData(-1); // Cursor is null, no calibration app installed
        else if (cursor.moveToLast()) { // Move to last in case there are multiple rows
            /*StringBuilder strBuild=new StringBuilder();
            strBuild.append(cursor.getString(cursor.getColumnIndex(CALIBRATED_POSITION_X))
                    + "~"+ cursor.getString(cursor.getColumnIndex(CALIBRATED_POSITION_Z))
                    + "~"+ cursor.getString(cursor.getColumnIndex(CALIBRATED_ROTATION_Y))
                    + "~"+ cursor.getString(cursor.getColumnIndex(BOUNDARY_0_X))
                    + "~"+ cursor.getString(cursor.getColumnIndex(BOUNDARY_0_Z))
                    + "~"+ cursor.getString(cursor.getColumnIndex(BOUNDARY_1_X))
                    + "~"+ cursor.getString(cursor.getColumnIndex(BOUNDARY_1_Z)));

            return strBuild.toString();*/
            return new CalibrationData(
                    1,
                    cursor.getFloat(cursor.getColumnIndex(CALIBRATED_POSITION_X)),
                    cursor.getFloat(cursor.getColumnIndex(CALIBRATED_POSITION_Z)),
                    cursor.getFloat(cursor.getColumnIndex(CALIBRATED_ROTATION_Y)),
                    cursor.getFloat(cursor.getColumnIndex(BOUNDARY_0_X)),
                    cursor.getFloat(cursor.getColumnIndex(BOUNDARY_0_Z)),
                    cursor.getFloat(cursor.getColumnIndex(BOUNDARY_1_X)),
                    cursor.getFloat(cursor.getColumnIndex(BOUNDARY_1_Z))
            );
        }
        else {
            return new CalibrationData(-2); // Cursor is empty, no calibration data;
        }
    }
}
