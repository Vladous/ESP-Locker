#include "rfid.h"
#include "config.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

static MFRC522 rfid(RFID_SDA, RFID_RST);
static String lastUID = "";
static bool cardAccepted = false;
static bool cardPresent = false;

void initRFID() {
  SPI.begin();          // SPI pro RC522
  rfid.PCD_Init();      // inicializace čtečky
  delay(50);
  Serial.println(F("✅ RFID inicializováno"));
}

String uidToString(MFRC522::Uid* uid) {
  String out = "";
  for (byte i = 0; i < uid->size; i++) {
    if (i > 0) out += " ";
    if (uid->uidByte[i] < 0x10) out += "0";
    out += String(uid->uidByte[i], HEX);
  }
  out.toUpperCase();
  return out;
}

void handleRFID() {
  cardAccepted = false;  // reset stavu
  cardPresent = false;
  lastUID = "";

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  cardPresent = true;
  lastUID = uidToString(&rfid.uid);
  Serial.print(F("💳 Načtená karta: "));
  Serial.println(lastUID);

  // Kontrola proti seznamu povolených
  for (size_t i = 0; i < ALLOWED_UIDS_COUNT; i++) {
    if (lastUID.equals(ALLOWED_UIDS[i])) {
      cardAccepted = true;
      Serial.println(F("✅ Povolená karta"));
      break;
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool rfidCardAccepted() {
  return cardAccepted;
}

String rfidGetLastUID() {
  return lastUID;
}

std::vector<String> getAllowedCards() {
  std::vector<String> cards;

  if (appConfig.allowedCardsJson.length() == 0) {
    // Použij statický seznam
    for (size_t i = 0; i < ALLOWED_UIDS_COUNT; i++) {
      cards.push_back(String(ALLOWED_UIDS[i]));
    }
    return cards;
  }

  // Jinak načti z JSON uloženého v EEPROM
  DynamicJsonDocument doc(512);
  if (deserializeJson(doc, appConfig.allowedCardsJson) == DeserializationError::Ok) {
    for (JsonVariant v : doc.as<JsonArray>()) {
      cards.push_back(v.as<String>());
    }
  }

  return cards;
}

void setAllowedCards(const std::vector<String>& cards) {
  DynamicJsonDocument doc(512);
  JsonArray arr = doc.to<JsonArray>();
  for (auto& s : cards) arr.add(s);

  String out;
  serializeJson(doc, out);
  appConfig.allowedCardsJson = out;
}

void rfidCheckCard(String& uid, bool& present, bool& accepted) {
  uid = lastUID;
  present = cardPresent;
  accepted = cardAccepted;
}
