package com.eecs149.block;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import android.util.Log;

/**
 * Created by stephanieho on 11/16/14.
 * references: http://www.kpbird.com/2013/07/android-notificationlistenerservice.html
 */
public class NLService extends NotificationListenerService {

    private String NLS_TAG = this.getClass().getSimpleName();

    private NLServiceReceiver nlServiceReceiver;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(ActivityUtils.APP_TAG, "### NLService started ###");
        nlServiceReceiver = new NLServiceReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(ActivityUtils.FILTER_ACTION);
        registerReceiver(nlServiceReceiver, filter);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        unregisterReceiver(nlServiceReceiver);
        stopService(new Intent(this, NLService.class));
    }


    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {

        Log.i(NLS_TAG, "**********  onNotificationPosted");
        Log.i(NLS_TAG, "ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText + "t" + sbn.getPackageName());
        Intent i = new Intent(ActivityUtils.NOTIFICATION_CHANGED);
        i.putExtra(ActivityUtils.EXTRA_NEW_NOTIF, sbn);
        sendBroadcast(i);

    }

    @Override
    public void onNotificationRemoved(StatusBarNotification sbn) {
        Log.i(NLS_TAG, "********** onNotificationRemoved");
        Log.i(NLS_TAG, "ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText + "t" + sbn.getPackageName());
//        Intent i = new Intent(ActivityUtils.NOTIFICATION_CHANGED);
//        i.putExtra("notification_event", "onNotificationRemoved :" + sbn.getPackageName() + "n");
//        sendBroadcast(i);
    }

    class NLServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.i(ActivityUtils.APP_TAG, "NLService received broadcast");
        }
    }


}
