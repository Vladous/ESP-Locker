# Přispívání do projektu ESP-Locker

Děkujeme, že chcete přispět k projektu ESP-Locker! 🙏

## Jak přispět

### 1. Nahlašování chyb

Pokud najdete chybu, prosím vytvořte GitHub Issue s následujícími informacemi:

- **Popis**: Co se stalo?
- **Očekávané chování**: Jak by to mělo fungovat?
- **Skutečné chování**: Co se stalo místo toho?
- **Hardware**: Jaký hardware používáte (ESP32, ESP8266, která deska)?
- **Software**: Verze Arduino IDE, ESP32 board support atd.
- **Kroky reprodukce**: Jak problém zopakovat?

### 2. Navrhování nových funkcí

Pokud máte nápady na nové funkce:

1. Otevřete GitHub Discussion nebo Issue
2. Popište požadovanou funkci
3. Vysvětlete, proč by byla užitečná
4. Diskutujte se správcem projektu

### 3. Přispívání kódem

#### Příprava

```bash
# Fork repozitáře na GitHubu

# Clone Vašeho forku
git clone https://github.com/YOUR-USERNAME/ESP-Locker.git
cd ESP-Locker

# Přidejte upstream
git remote add upstream https://github.com/Vladous/ESP-Locker.git

# Vytvořte feature branch
git checkout -b feature/moje-funkce
```

#### Vývoj

- Dodržujte styl kódu projektu
- Přidávejte komentáře k složitým funkcím (v češtině)
- Testujte na reálném hardwaru
- Aktualizujte dokumentaci v README.md pokud je potřeba

#### Commit

```bash
# Commit se smysluplnou zprávou
git commit -m "Přidán feature X - krátký popis"

# Push na Váš fork
git push origin feature/moje-funkce
```

#### Pull Request

1. Otevřete PR z Vašeho forku do [Vladous/ESP-Locker](https://github.com/Vladous/ESP-Locker)
2. Popište Co a Proč v PR popisu
3. Čekejte na review a zpětnou vazbu
4. Proveďte požadované změny
5. Po schválení bude sloučeno

## Pravidla kódu

### Formátování

```cpp
// Funkce s popisem
void mojeFunkce() {
  // Kód
}

// Konstanty
#define KONSTANTA 123

// Proměnné
int promena = 0;
```

### Komentáře

```cpp
// Jednoduché vysvětlení
int x = 5;

// Složitější logika vyžaduje detailnější komentář
// Toto je důležité pro pochopení
```

## Testování

Prosím testujte Vaše změny:

1. Nahrají se do ESP32/ESP8266 bez chyb?
2. Funguje na cílové desce?
3. Nerozbíjí to existující funkcionalitu?

## Dokumentace

- Pokud přidáváte nový feature, aktualizujte README.md
- Přidávejte komentáře do kódu
- Pište jasné commit zprávy

## Komunikace

- Buďte zdvořilý a respectful
- Anglické nebo české komentáře jsou OK
- Pomozte ostatním přispěvatelům

## Kontakt

Máte otázky? Otevřete GitHub Discussion nebo Issue.

---

**Děkujeme za Váš příspěvek! 🚀**
