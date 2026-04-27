#pragma once
#include <Arduino.h>
#include <vector>

// Inicializace čtečky
void initRFID();

// Hlavní smyčka čtečky (volat v loop)
void handleRFID();

// Vrací true, pokud byla přečtena povolená karta
bool rfidCardAccepted();

// Vrací poslední přečtené UID (jako String, i když nebyla povolená)
String rfidGetLastUID();

void rfidCheckCard(String& uid, bool& present, bool& accepted);

std::vector<String> getAllowedCards();
void setAllowedCards(const std::vector<String>& cards);