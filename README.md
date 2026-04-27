# ESP-Locker 🔐

Inteligentní zámek ovládaný přes **ESP32** s **NFC/RFID čtečkou** a **MQTT** komunikací. Projekt umožňuje vzdálené otevírání a zavírání zámku, správu autorizovaných karet a monitoring stavu zařízení přes MQTT.

---

## ✨ Hlavní funkce

- 🔑 **RFID/NFC autentizace** - Čtení karet a kontrola oprávnění
- 🌐 **WiFi a MQTT** - Vzdálená komunikace a ovládání
- 📊 **OLED Display** - Grafické zobrazení stavu a informací
- 🌡️ **Senzor teploty** - Monitoring prostředí (DHT11)
- 🔄 **OTA aktualizace** - Aktualizace firmware bez fyzického přístupu
- 📱 **Servo motor** - Elektromechanický zámek (SG90)
- 💾 **EEPROM config** - Ukládání nastavení mezi restarty
- 📡 **WiFi signál monitoring** - Zobrazení kvality signálu (RSSI)

---

## 🛠️ Hardware

### Součástky

| Komponenta | Model | Pin (ESP32) |
|-----------|-------|------------|
| Mikrokontrolér | ESP32 | - |
| RFID čtečka | RC522 | SDA: 5, SCK: 18, MISO: 23, MOSI: 19, RST: 4 |
| Servo motor | SG90 | GPIO1 (TX) |
| Teplotní senzor | DHT11 | GPIO25 |
| Display | OLED (SSD1306) | I2C |
| Napájení | 5V USB/PSU | - |

### Schéma připojení

```
ESP32 ← RC522 RFID čtečka
    ↓ I2C
  OLED Display (128x64)
    ↓ PWM
  Servo motor SG90
    ↓ GPIO25
  DHT11 (teplota/vlhkost)
```

---

## 📋 Požadavky

### Software

- **Arduino IDE** 1.8.13 nebo novější
- **ESP32 Board Support** (`esp32` v Arduino)
- **ESP8266 Board Support** (volitelně, pokud chcete ESP8266)

### Knihovny

Nainstalujte přes Arduino IDE → Manage Libraries:

```
- PubSubClient (Nick O'Leary)
- ESP32Servo (John K. Bennett) pro ESP32
- Adafruit SSD1306 (Adafruit Industries)
- Adafruit GFX Library (Adafruit Industries)
- DHT sensor library (Adafruit Industries)
- MFRC522 (GithubCommunity)
```

---

## 🚀 Instalace a Nastavení

### 1. Klonování repozitáře

```bash
git clone https://github.com/your-username/ESP-Locker.git
cd ESP-Locker
```

### 2. Konfigurace (`config.h`)

Upravte následující nastavení:

```cpp
// WiFi
#define WIFI_NAME "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"
#define WIFI_HOSTNAME "Locker"

// MQTT Server (např. Raspberry Pi)
#define MQTT_SERVER "192.168.10.1"
#define MQTT_PORT 1883

// Teplotní senzor
#define SENSOR_DHT
#define DHT_PIN 25

// Povolené RFID karty
const char* ALLOWED_UIDS[] = {
  "A9 54 53 D5",   // Vaše karta 1
  "12 34 56 78",   // Vaše karta 2
};
```

### 3. Nahrání do ESP32

1. Připojte ESP32 k počítači přes USB
2. V Arduino IDE: **Tools** → **Board** → **ESP32** (vyberte `ESP32 Dev Module`)
3. Nastavte port a rychlost (115200 baud)
4. Klikněte **Upload** (→)

---

## 📡 MQTT Témata

### Publikovaná témata (ze zařízení)

```
locker/status          → "locked" nebo "unlocked"
locker/temperature     → "23.5°C"
locker/humidity        → "65%"
locker/rssi            → "-55dB" (signál WiFi)
locker/rfid_uid        → "A9 54 53 D5" (poslední čtená karta)
locker/rfid_unknown    → "12 34 56 78" (nepovolená karta)
```

### Přijímaná témata (ovládání)

```
locker/lock            → "lock" (zavřít)
locker/unlock          → "unlock" (otevřít)
locker/config          → JSON nastavení
```

### Příklad MQTT příkazu

```bash
# Otevřít zámek
mosquitto_pub -h 192.168.10.6 -t "locker/unlock" -m "open"

# Zavřít zámek
mosquitto_pub -h 192.168.10.6 -t "locker/lock" -m "close"
```

---

## 📁 Struktura projektu

```
ESP-Locker/
├── ESP-Locker.ino          # Hlavní sketch
├── config.h                # Konfigurace (WiFi, MQTT, piny)
├── display.h/cpp           # OLED display ovládání
├── rfid.h/cpp              # RC522 RFID čtečka
├── servo.h/cpp             # Servo motor pro zámek
├── wifi_mqtt.h/cpp         # WiFi a MQTT komunikace
├── mqttHandler.h/cpp       # Obsluha MQTT zpráv
├── sensors.h/cpp           # DHT teplota/vlhkost
├── eeprom.h/cpp            # Správa konfigurace v EEPROM
├── ota.h/cpp               # OTA firmware aktualizace
├── bitmaps.h               # Ikony a obrázky pro display
└── README.md               # Tento soubor
```

---

## 🔧 Vývoj a Debugging

### Sériový monitor

1. V Arduino IDE: **Tools** → **Serial Monitor**
2. Nastavte rychlost na **115200 baud**
3. Sledujte debug zprávy

### Příklad výstupu

```
START
Loading config...
WiFi connecting...
Connected to: Internet_2.4GHz
MQTT connected
RFID ready
System ready - Unlocked
```

---

## 📝 Verze a Změny

- **v1.0** - Základní funkcionalita (RFID, MQTT, Servo)
- **v1.1** - Přidán alarm a senzor teploty

---

## 🤝 Příspívání

Příspěvky jsou vítány! Pro větší změny:

1. Vytvořte fork repozitáře
2. Vytvořte feature branch (`git checkout -b feature/AmazingFeature`)
3. Commitujte změny (`git commit -m 'Add AmazingFeature'`)
4. Pushujte na branch (`git push origin feature/AmazingFeature`)
5. Otevřete Pull Request

---

## 📄 Licence

Tento projekt je licencován pod **MIT License** - viz soubor [LICENSE](LICENSE) pro detaily.

---

## 📞 Kontakt

**GitHub:** [@Vladous](https://github.com/Vladous)

---

## ⚠️ Bezpečnostní poznámky

- 🔐 **Změňte výchozí RFID karty** v `config.h` před nasazením
- 🔑 **Zabezpečte WiFi** silným heslem
- 🛡️ **MQTT broker** by měl být zabezpečen (autentizace, TLS)
- 💾 Uchovávejte **offline kopii** seznamu autorizovaných karet

---

## 📚 Užitečné zdroje

- [ESP32 Arduino API](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [MQTT Dokumentace](https://mqtt.org/)
- [RC522 RFID Library](https://github.com/miguelbalboa/rfid)
- [PubSubClient Library](https://github.com/knolleary/pubsubclient)

---

**Vytvořeno s ❤️ pro IoT nadšence**
