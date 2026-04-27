#include "display.h"
#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// I2C – OLED 128 x 64
#define I2C_SDA    21
#define I2C_SCL    22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void displayInitialize() {
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED displej nenalezen"));
    return;
  }
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0x7F); // 0x00 - nejnižší, 0xFF - maximální
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
  displayShow();
}

void displayClear() {
  display.clearDisplay();
}

void displayShow() {
  display.display();  // refresh obsahu na OLED
}

void displayPrintln(const char* text, int16_t left, int16_t top, uint8_t size) {
  if (size > 0) {
    display.setTextSize(size);
  }
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  if (left >= 0 && top >= 0) {
    display.setCursor(left, top);
  }

  display.println(text);
}

void displayPrint(const char* text, int16_t left, int16_t top, uint8_t size) {
  if (size > 0) {
    display.setTextSize(size);
  }
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  if (left >= 0 && top >= 0) {
    display.setCursor(left, top);
  }

  display.print(text);
}

void drawImage(const ImageDef& img, int16_t left, int16_t top, uint16_t color) {
  display.drawBitmap(left, top, img.data, img.width, img.height, color);
}