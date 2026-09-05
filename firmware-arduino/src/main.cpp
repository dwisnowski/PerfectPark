#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

#include "config.h"
#include "parking_manager.h"
#include "web_server.h"

ParkingManager parkingManager;

void connectWifi() {
  WiFiManager wifiManager;
  wifiManager.setConfigPortalBlocking(true);
  wifiManager.setConnectTimeout(20);
  wifiManager.setConnectRetries(2);
  wifiManager.setCaptivePortalEnable(true);
  wifiManager.setTitle("PerfectPark WiFi Setup");

  Serial.println("Starting WiFiManager portal if needed...");
  if (!wifiManager.autoConnect(WIFI_PORTAL_NAME)) {
    Serial.println("WiFi connection failed, restarting...");
    delay(2000);
    ESP.restart();
  }

  Serial.print("Connected to WiFi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("PerfectPark v1 starting");

  parkingManager.begin();
  connectWifi();
  startWebServer(parkingManager);

  Serial.printf("Monitoring %u parking spot(s)\n", parkingManager.spotCount());
  Serial.printf("Detect range: %.1f ft, park target: ~%.1f ft\n", cmToFeet(DETECT_CM),
                cmToFeet((PARKED_MIN_CM + PARKED_MAX_CM) / 2.0f));
}

void loop() {
  parkingManager.update();
}
