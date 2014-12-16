package com.eecs149.block;

import java.util.HashMap;
import java.util.UUID;


public class GattAttributes {
    private static HashMap<UUID, String> attributes = new HashMap();
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