#include "wifi_manager.h"  // WiFi.h is already included through wifi.h
#include "config.h"
#include "leds.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

bool WiFiManager::connected = false;
unsigned long WiFiManager::lastReconnectAttempt = 0;

void WiFiManager::init() {
    Serial.println("[WIFI] Initializing WiFi...");
    Serial.printf("[WIFI] SSID: %s\n", WIFI_SSID);
    
    // Set WiFi mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);  // Clear any previous connection
    delay(100);
    
    // Set status LED to orange (connecting)
    LEDs::setWiFiStatus(1);  // 1 = orange (connecting)
    
    // Start connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[WIFI] Connecting");
    
    // Wait for connection with timeout
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {  // 20 seconds timeout
        delay(500);
        Serial.print(".");
        
        // Blink WiFi LED while connecting
        if (attempts % 2 == 0) {
            LEDs::setWiFiLED(COLOR_ORANGE);
        } else {
            LEDs::setWiFiLED(COLOR_OFF);
        }
        
        attempts++;
    }
    
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        LEDs::setWiFiStatus(2);  // 2 = green (connected)
        
        Serial.println("[WIFI] Connected successfully!");
        printNetworkInfo();
    } else {
        connected = false;
        LEDs::setWiFiStatus(3);  // 3 = red (error)
        
        Serial.println("[WIFI] Failed to connect!");
        Serial.printf("[WIFI] Status: %d\n", WiFi.status());
        
        // Print connection failure reason
        switch(WiFi.status()) {
            case WL_NO_SSID_AVAIL:
                Serial.println("[WIFI] SSID not found");
                break;
            case WL_CONNECT_FAILED:
                Serial.println("[WIFI] Connection failed");
                break;
            case WL_CONNECTION_LOST:
                Serial.println("[WIFI] Connection lost");
                break;
            /* case WL_WRONG_PASSWORD:
                Serial.println("[WIFI] Wrong password");
                break; */
            default:
                Serial.println("[WIFI] Unknown error");
                break;
        }
    }
}

bool WiFiManager::isConnected() {
    bool currentlyConnected = (WiFi.status() == WL_CONNECTED);
    
    // Update LED status if connection state changed
    if (currentlyConnected != connected) {
        connected = currentlyConnected;
        LEDs::setWiFiStatus(connected ? 2 : 3);  // Green if connected, red if not
        
        if (connected) {
            Serial.println("[WIFI] Connection restored");
            Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
        } else {
            Serial.println("[WIFI] Connection lost");
        }
    }
    
    return connected;
}

void WiFiManager::reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        unsigned long now = millis();
        
        // Only attempt reconnection every 5 seconds
        if (now - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = now;
            
            Serial.println("[WIFI] Attempting to reconnect...");
            LEDs::setWiFiStatus(1);  // Orange (connecting)
            
            WiFi.disconnect();
            delay(100);
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            
            // Quick connect attempt (5 seconds)
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 10) {
                delay(500);
                
                // Blink WiFi LED
                if (attempts % 2 == 0) {
                    LEDs::setWiFiLED(COLOR_ORANGE);
                } else {
                    LEDs::setWiFiLED(COLOR_OFF);
                }
                
                attempts++;
            }
            
            if (WiFi.status() == WL_CONNECTED) {
                connected = true;
                LEDs::setWiFiStatus(2);  // Green (connected)
                Serial.println("[WIFI] Reconnected successfully!");
                printNetworkInfo();
            } else {
                connected = false;
                LEDs::setWiFiStatus(3);  // Red (error)
                Serial.println("[WIFI] Reconnection failed");
            }
        }
    }
}

void WiFiManager::checkConnection() {
    if (!isConnected()) {
        reconnect();
    }
}

String WiFiManager::getIP() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

int WiFiManager::getSignalStrength() {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return -100;  // Very weak signal indicator when not connected
}

void WiFiManager::printNetworkInfo() {
    Serial.println("[WIFI] Network Information:");
    Serial.printf("[WIFI]   SSID: %s\n", WiFi.SSID().c_str());
    Serial.printf("[WIFI]   IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI]   Signal Strength: %d dBm\n", WiFi.RSSI());
    Serial.printf("[WIFI]   MAC Address: %s\n", WiFi.macAddress().c_str());
    Serial.printf("[WIFI]   Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("[WIFI]   Subnet: %s\n", WiFi.subnetMask().toString().c_str());
}