package com.eecs149.block;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.service.notification.StatusBarNotification;
import android.util.Log;
import android.view.View;
import android.widget.ListView;

import java.util.ArrayList;


public class BlockActivity extends Activity {

    private ListView lvNotifications;
    private NotificationAdapter adapterNotifs;

    private NotificationReceiver nReceiver;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_block);

        lvNotifications = (ListView) findViewById(R.id.lvNotifications);
        ArrayList<BlockNotification> blockNotifsList = new ArrayList<BlockNotification>();
        adapterNotifs = new NotificationAdapter(this, blockNotifsList);
        lvNotifications.setAdapter(adapterNotifs);

        // set receiver
        nReceiver = new NotificationReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(ActivityUtils.NOTIFICATION_CHANGED);
        registerReceiver(nReceiver, filter);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(nReceiver);
    }

    public void getNotifications(View view) {
        Log.i(ActivityUtils.APP_TAG, "GRABBING NOTIFICATIONS");
        Intent intent = new Intent(ActivityUtils.FILTER_ACTION);
        intent.putExtra(ActivityUtils.NLS_COMMAND, ActivityUtils.GET_NOTIFS);
        sendBroadcast(intent);
    }

    class NotificationReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            // add to adapter.
            // TODO : does not yet support removing from list
            Log.i(ActivityUtils.APP_TAG, "Received new item from NLService");
            StatusBarNotification sbn = intent.getParcelableExtra(ActivityUtils.EXTRA_NEW_NOTIF);
            BlockNotification newBlockNotif = new BlockNotification(sbn.getPackageName(), "", sbn.getPostTime());
            adapterNotifs.update(newBlockNotif);
        }
    }


//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.menu_main, menu);
//        return true;
//    }
//
//    @Override
//    public boolean onOptionsItemSelected(MenuItem item) {
//        // Handle action bar item clicks here. The action bar will
//        // automatically handle clicks on the Home/Up button, so long
//        // as you specify a parent activity in AndroidManifest.xml.
//        int id = item.getItemId();
//
//        //noinspection SimplifiableIfStatement
//        if (id == R.id.action_settings) {
//            return true;
//        }
//
//        return super.onOptionsItemSelected(item);
//    }
}
