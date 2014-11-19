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
import android.os.Bundle;
import android.os.IBinder;
import android.service.notification.StatusBarNotification;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Random;
import java.util.UUID;


public class BlockActivity extends Activity {

    private ListView lvNotifications;
    private TextView tvDeviceName;
    private TextView tvConnectionState;
    private TextView tvDataField;
    private Button btnConnectionToggle;
    private Button btnSendColor;
    private NotificationAdapter adapterNotifs;

    private String deviceName;
    private String deviceAddress;

    private BluetoothLeService btleService;
    private boolean connected = false;
    private BluetoothGattCharacteristic characteristicTX;
    private BluetoothGattCharacteristic characteristicRX;

    private NotificationReceiver notifReceiver;

    public final static UUID UUID_nRF_TX = UUID.fromString(GattAttributes.nRF_TX);
    public final static UUID UUID_nRF_RX = UUID.fromString(GattAttributes.nRF_RX);

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

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
//                clearUI();
            } else if (ActivityUtils.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                Log.i(ActivityUtils.APP_TAG, "=== GATT SERVICE DISCOVERED");

                // Show all the supported services and characteristics on the user interface.
                displayGattServices(btleService.getSupportedGattServices());
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
        btnSendColor = (Button) findViewById(R.id.btnSendColor);
        tvDeviceName = (TextView) findViewById(R.id.tvDeviceName);
        tvConnectionState = (TextView) findViewById(R.id.tvConnectionState);
        tvDataField = (TextView) findViewById(R.id.tvDataField);
        tvDeviceName.setText(deviceName);
        lvNotifications = (ListView) findViewById(R.id.lvNotifications);
        ArrayList<BlockNotification> blockNotifsList = new ArrayList<BlockNotification>();
        adapterNotifs = new NotificationAdapter(this, blockNotifsList);
        lvNotifications.setAdapter(adapterNotifs);
//        getActionBar().setDisplayHomeAsUpEnabled(true); // no action bar for now

        // set notifications receiver
        notifReceiver = new NotificationReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(ActivityUtils.NOTIFICATION_CHANGED);
        registerReceiver(notifReceiver, filter);

        // start bluetooth service
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, btServiceConnection, BIND_AUTO_CREATE);

        // start notification listener service
//        Log.i(ActivityUtils.APP_TAG, "### ATTEMPTING TO START NLSERVICE");
//        startService(new Intent(this, NLService.class));


    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(gattUpdateReceiver, makeGattUpdateIntentFilter());
        if (btleService != null) {
            final boolean result = btleService.connect(deviceAddress);
            Log.d(ActivityUtils.APP_TAG, "onResume, Connection request= " + result);
        }
        // TODO: same thing for notification listener

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

    // for testing
    public void sendColor(View view) {
        int choice = randInt(0, 4);
        String msg = getAppName(choice) + "\n";
        Log.d(ActivityUtils.APP_TAG, " ### Sending result=" + msg);
        final byte[] tx = msg.getBytes();
        if (connected) {
            if (characteristicTX == null) {
                Log.i(ActivityUtils.APP_TAG, "### STEPHANIE YOU MESSED UP");
            } else {
            Log.i(ActivityUtils.APP_TAG, "### NOT NULL");
        }
            characteristicTX.setValue(tx);
            btleService.writeCharacteristic(characteristicTX);
            btleService.setCharacteristicNotification(characteristicRX, true);
        }
    }

    private String getAppName(int choice) {
        switch(choice){
            case 0:
                return "Facebook";
            case 1:
                return "Line";
            case 2:
                return "Gmail";
            case 3:
                return "Messenger";
            case 4:
                return "Hangouts";
            default:
                return "Facebook";
        }
    }

    //temp
    public static int randInt(int min, int max) {

        // NOTE: Usually this should be a field rather than a method
        // variable so that it is not re-seeded every call.
        Random rand = new Random();

        // nextInt is normally exclusive of the top value,
        // so add 1 to make it inclusive
        int randomNum = rand.nextInt((max - min) + 1) + min;
        return randomNum;
    }

    class NotificationReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            // add to adapter.
            // TODO : does not yet support removing from list
            Log.i(ActivityUtils.APP_TAG, "Received new item from NLService");
            StatusBarNotification sbn = intent.getParcelableExtra(ActivityUtils.EXTRA_NEW_NOTIF);
            BlockNotification newBlockNotif = new BlockNotification(sbn.getPackageName(),
                    sbn.getNotification().tickerText.toString(), sbn.getPostTime());
            adapterNotifs.update(newBlockNotif);
        }
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
            Log.i(ActivityUtils.APP_TAG, "### UUID CURRENT:" + uuid);
            Log.i(ActivityUtils.APP_TAG, GattAttributes.lookup(uuid, unknownServiceString));
            currentServiceData.put(
                    LIST_NAME, GattAttributes.lookup(uuid, unknownServiceString));

            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);


            if(GattAttributes.lookup(uuid, unknownServiceString) == "UART service UUID") {
                currentServiceData.put(LIST_UUID, uuid);
                gattServiceData.add(currentServiceData);

                // get characteristic when UUID matches RX/TX UUID
                characteristicTX = gattService.getCharacteristic(BluetoothLeService.UUID_nRF_TX);
                characteristicRX = gattService.getCharacteristic(BluetoothLeService.UUID_nRF_RX);
                Log.i(ActivityUtils.APP_TAG, "### iS NULL: "+ (characteristicTX == null));
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
