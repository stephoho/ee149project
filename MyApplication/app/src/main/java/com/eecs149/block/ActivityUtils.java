package com.eecs149.block;

import java.util.UUID;

/**
 * Created by stephanieho on 11/16/14.
 */
public class ActivityUtils {

    public static final String APP_TAG = "[BLOCK]";

    // intent keys
    public static final String FILTER_ACTION = "com.ee149.block.NOTIFICATION_LISTENER";

    public static final String NOTIFICATION_CHANGED = "com.ee149.block.NOTIFICATION_CHANGED";

    public static final String NLS_COMMAND = "com.ee149.block.COMMAND";

    public static final String GET_NOTIFS = "NLS.GETNOTIFS";

    public static final String EXTRA_NEW_NOTIF = "com.ee149.block.SBN_EXTRA";

    public static final String EXTRAS_DEVICE_NAME = "com.ee149.block.DEVICE_NAME";

    public static final String EXTRAS_DEVICE_ADDRESS = "com.ee149.block.DEVICE_ADDRESS";

    // bluetoothLeService
    public final static String ACTION_GATT_CONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED =
            "com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE =
            "com.example.bluetooth.le.ACTION_DATA_AVAILABLE";
    public final static String EXTRA_DATA =
            "com.example.bluetooth.le.EXTRA_DATA";
    public final static UUID UUID_HM_RX_TX =
            UUID.fromString(GattAttributes.HM_RX_TX);


}
