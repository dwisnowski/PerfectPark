#include "web_server.h"

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "ota_update.h"

namespace {

AsyncWebServer server(80);
ParkingManager *managerRef = nullptr;

String buildStatusJson() {
  JsonDocument doc;
  doc["device"] = "PerfectPark";
  doc["version"] = "1.0.0";
  doc["spot_count"] = managerRef ? managerRef->spotCount() : 0;
  doc["detect_ft"] = cmToFeet(DETECT_CM);
  doc["park_target_ft"] = cmToFeet((PARKED_MIN_CM + PARKED_MAX_CM) / 2.0f);
  doc["updated_at"] = millis();

  if (managerRef) {
    JsonArray spots = doc["spots"].to<JsonArray>();
    for (uint8_t i = 0; i < managerRef->spotCount(); ++i) {
      const SpotReading &reading = managerRef->reading(i);
      JsonObject spot = spots.add<JsonObject>();
      spot["id"] = reading.id;
      spot["label"] = reading.label;
      spot["state"] = spotStateToString(reading.state);
      spot["occupied"] = reading.occupied;
      spot["valid"] = reading.valid;
      if (reading.valid) {
        spot["distance_cm"] = reading.distanceCm;
        spot["distance_ft"] = reading.distanceFt;
      } else {
        spot["distance_cm"] = nullptr;
        spot["distance_ft"] = nullptr;
      }
      spot["updated_at"] = reading.updatedAtMs;
    }
  }

  String payload;
  serializeJson(doc, payload);
  return payload;
}

void handleApiStatus(AsyncWebServerRequest *request) {
  request->send(200, "application/json", buildStatusJson());
}

void handleNotFound(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
    return;
  }

  if (LittleFS.exists("/index.html")) {
    request->send(LittleFS, "/index.html", "text/html");
    return;
  }

  request->send(404, "text/plain", "Not found");
}

}  // namespace

void startWebServer(ParkingManager &manager) {
  managerRef = &manager;

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
  } else {
    Serial.println("LittleFS mounted");
  }

  server.on("/api/status", HTTP_GET, handleApiStatus);
  registerOtaHttpRoutes(server);

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("max-age=600");
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Web server running at http://");
  Serial.println(WiFi.localIP());
}
