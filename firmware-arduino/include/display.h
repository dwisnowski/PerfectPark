#pragma once

#include <IPAddress.h>

void beginDisplay();
void showWifiSetupInstructions();
void showWifiConnected(const char *ssid, const IPAddress &ip);
