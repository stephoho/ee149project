package com.eecs149.block;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import android.util.Log;

/**
 * Created by stephanieho on 11/16/14.
 * references: http://www.kpbird.com/2013/07/android-notificationlistenerservice.html
 */
public class NLService extends NotificationListenerService {

    private String NLS_TAG = this.getClass().getSimpleName();
    public static String NLS_POST = "NOTIF_POST";
    public static String NLS_REMOVE = "NOTIF_REMOVE";

    private NLServiceReceiver nlServiceReceiver;

    public static boolean isNotificationAccessEnabled = false;

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
        Log.i(ActivityUtils.APP_TAG, "### NLService destroyed ###");
        unregisterReceiver(nlServiceReceiver);
        stopService(new Intent(this, NLService.class));
    }

    @Override
    public boolean onUnbind(Intent mIntent) {
        boolean mOnUnbind = super.onUnbind(mIntent);
        isNotificationAccessEnabled = false;
        return mOnUnbind;
    }

    @Override
    public IBinder onBind(Intent mIntent) {
        IBinder mIBinder = super.onBind(mIntent);
        isNotificationAccessEnabled = true;
        return mIBinder;
    }


    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {

        Log.i(NLS_TAG, "**********  onNotificationPosted");
        Log.i(NLS_TAG, "ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText + "t" + sbn.getPackageName());

        // put the time, content, and app package into the intent.
        // Notes: there are some problems sending the entire sbn
        // see: http://stackoverflow.com/questions/20929107/android-broadcasting-parcelable-data
        // for our use case, packageName, postTime, and contents should be enough

        // clearable sbn only because don't want stuck notifications on [BLOCK]
        if (sbn.isClearable()) {
            Intent i = new Intent(ActivityUtils.NOTIFICATION_CHANGED);
            i.putExtra(ActivityUtils.EXTRA_NOTIF_ACTION_TYPE, NLS_POST);
            i.putExtra(ActivityUtils.EXTRA_NOTIF_PACKAGE_NAME, sbn.getPackageName());
            i.putExtra(ActivityUtils.EXTRA_NOTIF_WHEN, sbn.getNotification().when);
            i.putExtra(ActivityUtils.EXTRA_NOTIF_CONTENT, sbn.getNotification().tickerText.toString());
            sendBroadcast(i);
        }
    }

    @Override
    public void onNotificationRemoved(StatusBarNotification sbn) {
        Log.i(NLS_TAG, "********** onNotificationRemoved");
        Log.i(NLS_TAG, "ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText + "t" + sbn.getPackageName());
        Intent i = new Intent(ActivityUtils.NOTIFICATION_CHANGED);
        i.putExtra(ActivityUtils.EXTRA_NOTIF_ACTION_TYPE, NLS_REMOVE);
        i.putExtra(ActivityUtils.EXTRA_NOTIF_PACKAGE_NAME, sbn.getPackageName());
        sendBroadcast(i);
    }

    class NLServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.i(ActivityUtils.APP_TAG, "NLService received broadcast");
        }
    }


}
