#pragma once

class AsyncWebServer;

void beginOtaUpdates();
void registerOtaHttpRoutes(AsyncWebServer &server);
void serviceOtaUpdates();
