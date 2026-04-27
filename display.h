#pragma once
#include "bitmaps.h"
#include <Arduino.h>

void displayInitialize();
void displayClear();
void displayShow();

// sjednocené typy, defaulty pro nepovinné argumenty
void displayPrintln(const char* text, int16_t left = -1, int16_t top = -1, uint8_t size = 0);
void displayPrint(const char* text, int16_t left = -1, int16_t top = -1, uint8_t size = 0);

// Funkce pro obrázky
void drawImage(const ImageDef& img, int16_t left, int16_t top, uint16_t color);