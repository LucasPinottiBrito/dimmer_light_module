#pragma once
#include <WiFiManager.h>

class WiFiManagerHelper {
public:
    static void connect() {
        WiFiManager wifiManager;
        wifiManager.autoConnect("SmartRoom_Dimmer");
    }
};