#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>  // CRITICAL: Include WiFi.h HERE in the header file!

class WiFiManager {
public:
    static void init();
    static bool isConnected();
    static void reconnect();
    static void checkConnection();
    static String getIP();
    static int getSignalStrength();
    static void printNetworkInfo();
    
private:
    static bool connected;
    static unsigned long lastReconnectAttempt;
    static const unsigned long reconnectInterval = 5000; // 5 seconds between reconnect attempts
};

#endif // WIFI_H