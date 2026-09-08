#include "ota_update.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Update.h>

namespace {

constexpr char OTA_HOSTNAME[] = "perfectpark";
constexpr uint32_t REBOOT_DELAY_MS = 1500;

AsyncWebServerRequest *activeRequest = nullptr;
String uploadError;
int uploadErrorStatus = 500;
bool uploadActive = false;
bool updateStarted = false;
bool filesystemUnmounted = false;
uint32_t rebootAtMs = 0;

bool originAllowed(AsyncWebServerRequest *request) {
  if (!request->hasHeader("Origin")) {
    return true;
  }
  if (!request->hasHeader("Host")) {
    return false;
  }

  const String origin = request->getHeader("Origin")->value();
  const String expectedOrigin = "http://" + request->getHeader("Host")->value();
  return origin == expectedOrigin;
}

void setUploadError(const String &message, int status = 500) {
  if (uploadError.isEmpty()) {
    uploadError = message;
    uploadErrorStatus = status;
  }
  Serial.print("OTA upload error: ");
  Serial.println(message);
}

void scheduleReboot() {
  rebootAtMs = millis() + REBOOT_DELAY_MS;
}

void beginHttpUpload(AsyncWebServerRequest *request, int command, bool isFilesystem,
                     const String &filename) {
  if (uploadActive) {
    return;
  }

  uploadActive = true;
  activeRequest = request;
  uploadError = "";
  uploadErrorStatus = 500;
  updateStarted = false;
  filesystemUnmounted = false;

  if (!originAllowed(request)) {
    setUploadError("Cross-origin OTA requests are not allowed", 403);
    return;
  }

  String normalizedFilename = filename;
  normalizedFilename.toLowerCase();
  const char *expectedFilename = isFilesystem ? "littlefs.bin" : "firmware.bin";
  if (!normalizedFilename.endsWith(expectedFilename)) {
    setUploadError("Unexpected image filename", 400);
    return;
  }

  Serial.printf("HTTP OTA start: %s (%s)\n", isFilesystem ? "LittleFS" : "firmware",
                filename.c_str());

  if (isFilesystem) {
    LittleFS.end();
    filesystemUnmounted = true;
  }

  if (!Update.begin(UPDATE_SIZE_UNKNOWN, command)) {
    setUploadError(Update.errorString());
    return;
  }
  updateStarted = true;
}

void handleHttpUpload(AsyncWebServerRequest *request, const String &filename, size_t index,
                      uint8_t *data, size_t len, bool final, int command,
                      bool isFilesystem) {
  if (index == 0) {
    beginHttpUpload(request, command, isFilesystem, filename);
  }

  if (request != activeRequest || !uploadError.isEmpty()) {
    return;
  }

  if (!updateStarted) {
    setUploadError("Unable to initialize OTA update");
    return;
  }

  if (len > 0 && Update.write(data, len) != len) {
    setUploadError(Update.errorString());
    Update.abort();
    updateStarted = false;
    return;
  }

  if (!final) {
    return;
  }

  if (!Update.end(true)) {
    setUploadError(Update.errorString());
    updateStarted = false;
    return;
  }

  updateStarted = false;
  Serial.printf("HTTP OTA complete: %u bytes\n", static_cast<unsigned>(index + len));
}

void finishHttpUpload(AsyncWebServerRequest *request) {
  if (request != activeRequest) {
    request->send(409, "application/json", "{\"ok\":false,\"error\":\"OTA busy\"}");
    return;
  }

  const bool succeeded = uploadError.isEmpty() && !updateStarted;
  if (succeeded) {
    request->send(200, "application/json",
                  "{\"ok\":true,\"message\":\"Update complete; rebooting\"}");
    scheduleReboot();
  } else {
    String payload = "{\"ok\":false,\"error\":\"";
    payload += uploadError.isEmpty() ? "Incomplete OTA upload" : uploadError;
    payload += "\"}";
    request->send(uploadErrorStatus, "application/json", payload);
    if (filesystemUnmounted || updateStarted) {
      if (updateStarted) {
        Update.abort();
      }
      scheduleReboot();
    }
  }

  activeRequest = nullptr;
  uploadActive = false;
  updateStarted = false;
}

void registerUpdateRoute(AsyncWebServer &server, const char *path, int command,
                         bool isFilesystem) {
  server.on(
      path, HTTP_POST, finishHttpUpload,
      [command, isFilesystem](AsyncWebServerRequest *request, const String &filename,
                              size_t index, uint8_t *data, size_t len, bool final) {
        handleHttpUpload(request, filename, index, data, len, final, command, isFilesystem);
      });
}

}  // namespace

void beginOtaUpdates() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.onStart([]() {
    const bool isFilesystem = ArduinoOTA.getCommand() != U_FLASH;
    Serial.printf("PlatformIO OTA start: %s\n", isFilesystem ? "LittleFS" : "firmware");
    if (isFilesystem) {
      LittleFS.end();
    }
  });
  ArduinoOTA.onEnd([]() { Serial.println("\nPlatformIO OTA complete"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    const unsigned int percent = total == 0 ? 0 : (progress * 100U) / total;
    Serial.printf("\rPlatformIO OTA progress: %u%%", percent);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("\nPlatformIO OTA error [%u]\n", error);
    LittleFS.begin(false);
  });
  ArduinoOTA.begin();

  Serial.print("OTA ready at ");
  Serial.print(OTA_HOSTNAME);
  Serial.println(".local");
}

void registerOtaHttpRoutes(AsyncWebServer &server) {
  registerUpdateRoute(server, "/api/update/firmware", U_FLASH, false);
  registerUpdateRoute(server, "/api/update/filesystem", U_SPIFFS, true);
}

void serviceOtaUpdates() {
  if (!uploadActive && rebootAtMs == 0) {
    ArduinoOTA.handle();
  }

  if (rebootAtMs != 0 && static_cast<int32_t>(millis() - rebootAtMs) >= 0) {
    Serial.println("Rebooting after OTA update...");
    Serial.flush();
    ESP.restart();
  }
}
