package com.eecs149.block;


/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.util.Log;

import java.util.HashMap;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class GattAttributes {
    private static HashMap<String, String> attributes = new HashMap();
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";

    public static String UART_SERVICE = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
    public static String nRF_TX = "6e400002-B5A3-F393-E0A9-E50E24DCCA9E".toLowerCase();
    public static String nRF_RX = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E".toLowerCase();

    static {
        attributes.put(nRF_TX, "TX data");
        attributes.put(nRF_RX, "RX data");
        attributes.put(UART_SERVICE, "UART service UUID");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        Log.i(ActivityUtils.APP_TAG, "==== NAME: " + name);
        return name == null ? defaultName : name;
    }
}