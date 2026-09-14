#include "display.h"

#include <Arduino_GFX_Library.h>

namespace {

constexpr uint16_t BACKGROUND_COLOR = BLACK;
constexpr uint16_t HEADING_COLOR = RGB565(69, 194, 255);
constexpr uint16_t ACCENT_COLOR = RGB565(255, 196, 61);
constexpr uint16_t SUCCESS_COLOR = RGB565(80, 220, 120);

Arduino_DataBus *displayBus =
    new Arduino_ESP32SPI(45 /* DC */, 21 /* CS */, 38 /* SCK */, 39 /* MOSI */);
Arduino_GFX *display =
    new Arduino_ST7789(displayBus, 47 /* RST */, 0 /* rotation */, false /* IPS */,
                       172 /* width */, 320 /* height */, 34 /* col offset 1 */,
                       0 /* row offset 1 */, 34 /* col offset 2 */,
                       0 /* row offset 2 */);
bool displayReady = false;

void printLine(const char *text, int16_t y, uint8_t size = 1,
               uint16_t color = WHITE) {
  display->setCursor(10, y);
  display->setTextColor(color);
  display->setTextSize(size);
  display->println(text);
}

}  // namespace

void beginDisplay() {
  if (!display->begin()) {
    Serial.println("LCD initialization failed");
    return;
  }

  displayReady = true;
  display->fillScreen(BACKGROUND_COLOR);
  display->setTextWrap(false);
}

void showWifiSetupInstructions() {
  if (!displayReady) {
    return;
  }

  display->fillScreen(BACKGROUND_COLOR);
  printLine("PerfectPark", 14, 2, HEADING_COLOR);
  printLine("WiFi Setup", 39, 2, WHITE);
  display->drawFastHLine(10, 66, 152, HEADING_COLOR);

  printLine("1. Open WiFi settings", 82);
  printLine("   on your phone.", 98);

  printLine("2. Join this network:", 126);
  printLine("PerfectPark-Setup", 144, 1, ACCENT_COLOR);

  printLine("3. Follow the setup", 174);
  printLine("   page and enter your", 190);
  printLine("   home WiFi password.", 206);

  printLine("If no page appears,", 236);
  printLine("open 192.168.4.1", 252, 1, ACCENT_COLOR);

  printLine("Waiting for setup...", 290, 1, HEADING_COLOR);
}

void showWifiConnected(const char *ssid, const IPAddress &ip) {
  if (!displayReady) {
    return;
  }

  display->fillScreen(BACKGROUND_COLOR);
  printLine("PerfectPark", 18, 2, HEADING_COLOR);
  printLine("WiFi connected!", 58, 2, SUCCESS_COLOR);

  printLine("Network:", 102);
  printLine(ssid, 120, 1, WHITE);

  printLine("Dashboard address:", 158);
  printLine(ip.toString().c_str(), 178, 2, ACCENT_COLOR);

  printLine("You can close WiFi", 224);
  printLine("settings now.", 240);
}
