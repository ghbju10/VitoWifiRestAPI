# ESP32 Vito Controller

Ein ESP32-basiertes IoT-Projekt zur Anbindung und Steuerung von **Viessmann Heizungsanlagen** über **Optolink / VitoWiFi**.  
Das System liest Heizungs-Datapunkte zyklisch aus, stellt sie über verschiedene Schnittstellen bereit und ermöglicht – je nach Datapunkt – auch das Schreiben von Sollwerten.

---

## ✨ Features

### 🔌 Heizungs-Anbindung
- Kommunikation mit Viessmann Heizungen über **Optolink (VitoWiFi-Protokoll)**
- Zentrales Datapoint-Management (Adresse, Länge, Konvertierung)
- Unterstützung für **Read- und Write-Datapoints** inkl. Min/Max-Validierung
- Startup-Delay und Poll-Intervall konfigurierbar

---

### 🌐 Netzwerk & System
- WLAN-Anbindung (statische IP oder DHCP)
- **WiFiManager Fallback** bei fehlender Konfiguration
- Regelmäßiger **Gateway-Ping** zur Netzwerküberwachung
- Automatischer Neustart bei wiederholtem Netzwerkausfall
- **mDNS** Unterstützung (Hostname im Netzwerk erreichbar)

---

### 🧠 Systemstatus & Monitoring
- Laufzeit (Uptime als String & Sekunden)
- Heap-Überwachung (freier Heap, minimaler Heap, Fragmentierung)
- WLAN-Status, RSSI, IP-Adresse
- Reset-Grund (PowerOn, WDT, Panic, Software, …)

---

### 🌍 REST API
- `GET /status` – System- und Heizungsstatus als JSON
- `GET /get?dp=<name>` – Datapoint-Details und aktueller Wert
- `POST /set?dp=<name>` – Schreiben eines Datapoints (JSON)
- `POST /reboot` – Neustart des ESP32

---

### 📡 UDP (inkl. Loxone)

Die UDP-Komponenten können **direkt zur Integration in Loxone** (z. B. Miniserver mit virtuellen UDP-Ein-/Ausgängen) verwendet werden.

#### UDP Sender
- Periodisches Senden ausgewählter Datapoints
- Frei definierbare Payload-Templates (`<v>` Platzhalter)
- Konfigurierbarer Ziel-Host & Port
- **Kompatibel mit Loxone virtuellen UDP-Eingängen**

Beispiel (ESP32 → Loxone):
```
VITO_TEMP_AUSSEN:12.3
```

#### UDP Server
- Empfang einfacher Steuerbefehle per UDP
- Format: `set:<datapoint>:<value>`
- Direkte Übergabe an die Vito-Write-Logik
- **Geeignet für Loxone virtuelle UDP-Ausgänge**

Beispiel (Loxone → ESP32):
```
set:warmwasserSollTemp:50
```

---

### 🧾 MQTT
- MQTT Client mit automatischem Reconnect
- Retained Messages
- Pro Datapoint aktivierbar
- Frei definierbare Topic- und Payload-Templates
- Ideal für Home Assistant, InfluxDB, Grafana

---

### ⏱️ Zeit & NTP
- Asynchroner NTP Client
- Zeitzonen-Unterstützung
- Automatische Sommerzeit (EU)
- Zeitstempel in Logs

---

### 📟 Logging
- Serial Logging
- Telnet Live-Logging
- Log-Level: DEBUG, INFO, WARN, ERROR
- Optional mit Zeitstempel

---

### 🔐 OTA
- Arduino OTA Updates
- Watchdog-Deaktivierung während OTA
- Automatischer Neustart nach erfolgreichem Update

---

### 🛡️ Stabilität
- ESP32 Task-Watchdog
- Netzwerküberwachung
- Automatischer Reboot bei Fehlern

---

## 🛠️ Installation & Build (PlatformIO)

### Voraussetzungen
- VS Code + PlatformIO Extension
- ESP32 Board (z. B. ESP32 Dev Module)
- Optolink Interface
- Viessmann Heizung mit Optolink

---

### Projekt einrichten

1. Repository klonen:
```bash
git clone https://github.com/<user>/<repo>.git
```
2. Projekt in VS Code öffnen  
3. PlatformIO initialisiert die Umgebung automatisch

---

### Konfiguration

1. Datei kopieren und umbenennen:
```
Configuration.example.h → Configuration.h
```
2. WLAN-, Netzwerk-, MQTT- und UDP-Parameter anpassen
3. `Configuration.h` **nicht** ins Git-Repository einchecken

---

### Build & Upload

```bash
pio run
pio run -t upload
```

---

### Erststart
- ESP32 verbindet sich mit dem WLAN
- Falls keine Zugangsdaten vorhanden sind, startet **WiFiManager**
- Nach dem Startup-Delay beginnen:
  - Heizungsabfrage
  - REST API
  - UDP / MQTT Kommunikation

---

### OTA Update
- OTA ist nach dem ersten Flash aktiv
- ESP32 erscheint automatisch als Netzwerk-Port
- Firmware-Updates können kabellos eingespielt werden

---

## ⚙️ Konfiguration (Details)

Die Datei `Configuration.example.h` enthält **alle projektweiten Konfigurationsparameter**.

👉 **Wichtig:**  
Vor dem Kompilieren muss diese Datei **lokal kopiert und umbenannt werden**:

```
Configuration.example.h  →  Configuration.h
```

Die Datei `Configuration.h` ist bewusst **nicht Teil des Git-Repositories**, da sie sensible Daten (WLAN, MQTT) enthält.

---

## 🚀 Typische Anwendungsfälle
- Home-Assistant Integration
- Loxone Heizungsintegration
- Heizungs-Monitoring
- Fernsteuerung von Sollwerten
- Datenexport via MQTT, UDP oder REST

---

## ⚠️ Hinweis
⚠️ Schreibzugriffe auf Heizungsparameter können Fehlfunktionen verursachen.  
Nur Datapoints freigeben, deren Bedeutung bekannt ist.

---

## 📜 Lizenz
Open Source – Lizenz bitte ergänzen.

---

## 🙌 Credits
- VitoWiFi / Optolink
- Arduino / ESP32 Community
