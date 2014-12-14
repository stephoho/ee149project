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
import java.util.UUID;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class GattAttributes {
    private static HashMap<UUID, String> attributes = new HashMap();
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";
//    UUID=3c43ec53-be4a-499a-bc15-5206a7258106
    public static UUID UART_SERVICE = UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    public static UUID nRF_TX = UUID.fromString("6e400002-B5A3-F393-E0A9-E50E24DCCA9E".toLowerCase());
    public static UUID nRF_RX = UUID.fromString("6E400003-B5A3-F393-E0A9-E50E24DCCA9E".toLowerCase());

    static {
        attributes.put(nRF_TX, "TX data");
        attributes.put(nRF_RX, "RX data");
        attributes.put(UART_SERVICE, "UART service UUID");
    }

    public static String lookup(String uuidStr, String defaultName) {
        String name = attributes.get(UUID.fromString(uuidStr));
        return name == null ? defaultName : name;
    }
}