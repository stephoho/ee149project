package com.eecs149.block;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import org.joda.time.DateTime;
import org.joda.time.LocalDateTime;
import org.joda.time.Minutes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;


public class BlockActivity extends Activity {

    private ListView lvNotifications;
    private TextView tvDeviceName;
    private TextView tvConnectionState;
    private TextView tvDataField;
    private Button btnConnectionToggle;
    private NotificationAdapter adapterNotifs;

    private String deviceName;
    private String deviceAddress;

    private BluetoothLeService btleService;
    private boolean connected = false;
    private BluetoothGattCharacteristic characteristicTX;
    private BluetoothGattCharacteristic characteristicRX;

    private NotificationReceiver notifReceiver;

    public final static UUID UUID_nRF_TX = GattAttributes.nRF_TX;
    public final static UUID UUID_nRF_RX = GattAttributes.nRF_RX;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";
    private final String REMOVE = "remove";
    private final String POST = "post";

    // manage bluetoothLeService lifecycle
    private final ServiceConnection btServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            btleService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!btleService.initialize()) {
                Log.e(ActivityUtils.APP_TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            btleService.connect(deviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            btleService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (ActivityUtils.ACTION_GATT_CONNECTED.equals(action)) {
                Log.i(ActivityUtils.APP_TAG, "=== GATT CONNECTED");
                connected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (ActivityUtils.ACTION_GATT_DISCONNECTED.equals(action)) {
                Log.i(ActivityUtils.APP_TAG, "=== GATT DISCONNECTED");
                connected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
            } else if (ActivityUtils.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                Log.i(ActivityUtils.APP_TAG, "=== GATT SERVICE DISCOVERED");
                // Show all the supported services and characteristics on the user interface.
                displayGattServices(btleService.getSupportedGattServices());

                // send device name and seed time (minutes)
                sendInitial();
            } else if (ActivityUtils.ACTION_DATA_AVAILABLE.equals(action)) {
                Log.i(ActivityUtils.APP_TAG, "=== GATT DATA AVAILABLE");
//                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_block);

        // get intent from device scan/discovery
        final Intent intent = getIntent();
        deviceName = intent.getStringExtra(ActivityUtils.EXTRAS_DEVICE_NAME);
        deviceAddress = intent.getStringExtra(ActivityUtils.EXTRAS_DEVICE_ADDRESS);

        // set up views
        btnConnectionToggle = (Button) findViewById(R.id.btnConnectionToggle);
        tvDeviceName = (TextView) findViewById(R.id.tvDeviceName);
        tvConnectionState = (TextView) findViewById(R.id.tvConnectionState);
        tvDataField = (TextView) findViewById(R.id.tvDataField);
        tvDeviceName.setText(deviceName);
        lvNotifications = (ListView) findViewById(R.id.lvNotifications);
        ArrayList<BlockNotification> blockNotifsList = new ArrayList<BlockNotification>();
        adapterNotifs = new NotificationAdapter(this, blockNotifsList);
        lvNotifications.setAdapter(adapterNotifs);

        // start bluetooth service
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, btServiceConnection, BIND_AUTO_CREATE);

    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(gattUpdateReceiver, makeGattUpdateIntentFilter());
        if (btleService != null) {
            final boolean result = btleService.connect(deviceAddress);
            Log.d(ActivityUtils.APP_TAG, "onResume, Connection request= " + result);
        }

        // set notifications receiver
        notifReceiver = new NotificationReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(ActivityUtils.NOTIFICATION_CHANGED);
        registerReceiver(notifReceiver, filter);
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(gattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // bluetooth service
        unbindService(btServiceConnection);
        btleService = null;
        // notification receiver
        unregisterReceiver(notifReceiver);
    }


    public void toggleConnection(View view) {
        if (connected) {
            btleService.disconnect();
            btnConnectionToggle.setText(R.string.action_connect);


        } else {
            btleService.connect(deviceAddress);
            btnConnectionToggle.setText(R.string.action_disconnect);
        }
    }


    class NotificationReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            // add to adapter.
            Log.i(ActivityUtils.APP_TAG, "Received new item from NLService");
            handleBroadcastIntent(intent);
            sendToBlock(intent);
        }
    }

    private void handleBroadcastIntent(Intent nlsIntent) {
        String actionType = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_ACTION_TYPE);
        if (actionType.equals(NLService.NLS_POST)) {
            String packageName = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_PACKAGE_NAME);
            String content = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_CONTENT);
            long timeStamp = nlsIntent.getLongExtra(ActivityUtils.EXTRA_NOTIF_WHEN, System.currentTimeMillis());

            BlockNotification newBlockNotif = new BlockNotification(packageName, content, timeStamp);
            adapterNotifs.update(newBlockNotif);

        } else if (actionType.equals(NLService.NLS_REMOVE)) {
            String packageName = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_PACKAGE_NAME);
            adapterNotifs.remove(packageName);
        }
    }

    private void sendToBlock(Intent nlsIntent) {
        String msg = makeDatagram(nlsIntent);
        System.out.println("#### SENDING: " + msg);
        byte[] tx = null;
        try {
            tx = msg.getBytes("US-ASCII");
        } catch (Exception e) {
            Log.d(ActivityUtils.APP_TAG, "THIS THING SUCKS OR YOU MESSED UP");
        }
        if (connected) {
            characteristicTX.setValue(tx);
            tvDataField.setText("SENT: " + msg);
            btleService.writeCharacteristic(characteristicTX);
            btleService.setCharacteristicNotification(characteristicRX, true);
        }
    }


    private void sendInitial() {
        String deviceName = getDeviceName();
        LocalDateTime now = new LocalDateTime();
        int minutesFromMidnight = Minutes.minutesBetween(now.withMillisOfDay(0),
                LocalDateTime.now()).getMinutes();
        String msg=  deviceName + ":" + minutesFromMidnight;
        byte[] tx = null;
        try {
            tx = msg.getBytes("US-ASCII");
        } catch (Exception e) {
            Log.d(ActivityUtils.APP_TAG, "THIS THING SUCKS OR YOU MESSED UP");
        }
        if (connected) {
            characteristicTX.setValue(tx);
            tvDataField.setText("SENT: " + msg);
            btleService.writeCharacteristic(characteristicTX);
            btleService.setCharacteristicNotification(characteristicRX, true);
        }

    }

    public String getDeviceName() {
        String manufacturer = Build.MANUFACTURER;
        String model = Build.MODEL;
        if (model.startsWith(manufacturer)) {
            return capitalize(model);
        } else {
            return capitalize(manufacturer) + " " + model;
        }
    }

    private String capitalize(String s) {
        if (s == null || s.length() == 0) {
            return "";
        }
        char first = s.charAt(0);
        if (Character.isUpperCase(first)) {
            return s;
        } else {
            return Character.toUpperCase(first) + s.substring(1);
        }
    }

    private String makeDatagram(Intent nlsIntent) {
        String pckg = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_PACKAGE_NAME);
        String app = BlockNotification.getShortName(pckg);
        String actionType = nlsIntent.getStringExtra(ActivityUtils.EXTRA_NOTIF_ACTION_TYPE);
        String action = "";
        if (actionType.equals((NLService.NLS_POST))) {
            action = POST;
        } else if (actionType.equals((NLService.NLS_REMOVE))) {
            action = REMOVE;
        }
        return app + ";" + action;
    }


    // update device connection state
    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvConnectionState.setText(resourceId);
            }
        });
    }

    // display most recently sent/received data
    private void displayData(String data) {
        if (data != null) {
            tvDataField.setText(data);
        }
    }

    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        String unknownServiceString = getResources().getString(R.string.unknown_service);
        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();


        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            HashMap<String, String> currentServiceData = new HashMap<String, String>();
            uuid = gattService.getUuid().toString();
            currentServiceData.put(
                    LIST_NAME, GattAttributes.lookup(uuid, unknownServiceString));

            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);

            if (GattAttributes.lookup(uuid, unknownServiceString).contains("UART")) {
                currentServiceData.put(LIST_UUID, uuid);
                gattServiceData.add(currentServiceData);

                // get characteristic when UUID matches RX/TX UUID
                characteristicTX = gattService.getCharacteristic(BluetoothLeService.UUID_nRF_TX);
                characteristicRX = gattService.getCharacteristic(BluetoothLeService.UUID_nRF_RX);
                break;
            }
        }
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }


//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.menu_block_activity, menu);
//        if (connected) {
//            menu.findItem(R.id.menu_connect).setVisible(false);
//            menu.findItem(R.id.menu_disconnect).setVisible(true);
//        } else {
//            menu.findItem(R.id.menu_connect).setVisible(true);
//            menu.findItem(R.id.menu_disconnect).setVisible(false);
//        }
//        return true;
//    }
//
//    @Override
//    public boolean onOptionsItemSelected(MenuItem item) {
//        switch(item.getItemId()) {
//            case R.id.menu_connect:
//                mBluetoothLeService.connect(mDeviceAddress);
//                return true;
//            case R.id.menu_disconnect:
//                mBluetoothLeService.disconnect();
//                return true;
//            case android.R.id.home:
//                onBackPressed();
//                return true;
//        }
//        return super.onOptionsItemSelected(item);
//    }
//    public void getNotifications(View view) {
//        Log.i(ActivityUtils.APP_TAG, "GRABBING NOTIFICATIONS");
//        Intent intent = new Intent(ActivityUtils.FILTER_ACTION);
//        intent.putExtra(ActivityUtils.NLS_COMMAND, ActivityUtils.GET_NOTIFS);
//        sendBroadcast(intent);
//    }
}
