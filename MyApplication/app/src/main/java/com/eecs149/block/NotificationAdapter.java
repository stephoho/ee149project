package com.eecs149.block;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by stephanieho on 11/16/14.
 */


public class NotificationAdapter extends ArrayAdapter<BlockNotification> {

    ArrayList<BlockNotification> blockNotifs;

    public NotificationAdapter(Context context, ArrayList<BlockNotification> notifList) {
        super(context, 0, notifList);
        blockNotifs = notifList;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        BlockNotification notif = getItem(position);

        if (convertView == null) {
            LayoutInflater inflater = LayoutInflater.from(getContext());
            convertView = inflater.inflate(R.layout.item_notification, parent, false);
        }

        TextView tvAppName = (TextView) convertView.findViewById(R.id.tvNotificationAppName);
        TextView tvContent = (TextView) convertView.findViewById(R.id.tvNotificationContent);
        TextView tvTime = (TextView) convertView.findViewById(R.id.tvNotificationTime);

        tvAppName.setText(notif.getAppName());
        tvContent.setText(notif.getContent());
        tvTime.setText(notif.getTimeString());

        return convertView;
    }

    public void update(BlockNotification newNotif) {
        add(newNotif);
    }

    public void remove(String packageName) {
        // remove all notifications from specified packageName
//        for (BlockNotification notif : blockNotifs) {
//            if (notif.getAppName().equals(packageName)) {
//                blockNotifs.remove(notif);
//            }
//        }
//        notifyDataSetChanged();

        clear();
    }

}
