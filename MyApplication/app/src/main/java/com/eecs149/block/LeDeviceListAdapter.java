package com.eecs149.block;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by stephanieho on 11/18/14.
 */

// Adapter for holding devices found through scanning.
public class LeDeviceListAdapter extends ArrayAdapter<BluetoothDevice> {
    private ArrayList<BluetoothDevice> arrayBTDevices;

    public LeDeviceListAdapter(Context context, ArrayList<BluetoothDevice> aBTDevices) {
        super(context, 0, aBTDevices);
        arrayBTDevices = aBTDevices;
    }


    public void addDevice(BluetoothDevice device) {
        if (!arrayBTDevices.contains(device)) {
            arrayBTDevices.add(device);
        }
    }

    public BluetoothDevice getDevice(int position) {
        return arrayBTDevices.get(position);
    }

    public void clear() {
        arrayBTDevices.clear();
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        // General ListView optimization code.
        if (convertView == null) {
            LayoutInflater inflater = LayoutInflater.from(parent.getContext());
            convertView = inflater.inflate(R.layout.listitem_device, parent, false);

            viewHolder = new ViewHolder();
            viewHolder.deviceAddress = (TextView) convertView.findViewById(R.id.device_address);
            viewHolder.deviceName = (TextView) convertView.findViewById(R.id.device_name);
            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        BluetoothDevice device = arrayBTDevices.get(position);
        final String deviceName = device.getName();
        if (deviceName != null && deviceName.length() > 0)
            viewHolder.deviceName.setText(deviceName);
        else
            viewHolder.deviceName.setText(R.string.unknown_device);
        viewHolder.deviceAddress.setText(device.getAddress());

        return convertView;
    }

    static class ViewHolder {
        TextView deviceName;
        TextView deviceAddress;
    }
}