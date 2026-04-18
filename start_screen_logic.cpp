#include "app_state.h"
#include "start_screen_logic.h"
#include "ui.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <stdio.h>
#include <time.h>
#include <esp_system.h>
#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>


// Konfiguration (Passe diese ggf. an)
const char* ssid = "o2-WLAN12";
const char* password = "C4YDRC76VPY3HVPV";
const char* apiUrl = "http://mawaqit-api.almofleh.com/api/v1/salahuddin-dormagen/prayer-times";
const char* TZ_INFO = "CET-1CEST,M3.5.0,M10.5.0/3";

#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

SPIClass hspi(HSPI);

static String lastText = "";

void start_screen_init() {
}

void start_screen_loop() {
  switch (currentStatus) {
    case ST_START:
      initAudio();
      currentStatus = ST_INIT_SD;
      break;

    case ST_INIT_SD:
      initSD();
      currentStatus = ST_CONNECT_WIFI;
      break;

    case ST_CONNECT_WIFI:
      static unsigned long lastCheck = 0;  // Prüft alle 1 Sekunde
      static unsigned long startTime = 0;  // Startzeit für Timeout
      static bool wifiStarted = false;

      if (!wifiStarted) {
        initWIFI();
        startTime = millis();  // Startzeit merken
        wifiStarted = true;
      }

      // Prüfen, ob WiFi verbunden ist
      if (millis() - lastCheck > 1000) {  // nur alle 1 Sekunde prüfen
        lastCheck = millis();

        if (WiFi.status() == WL_CONNECTED) {
          updateStatus("WiFi verbunden!");

          configTime(3600, 3600, "pool.ntp.org");
          setenv("TZ", TZ_INFO, 1);
          tzset();

          currentStatus = ST_GET_DATA;  // weiter zum nächsten Schritt
        } else {
          updateStatus("WiFi verbindet...");

          // Retry-Timeout: nach 20 Sekunden erneut versuchen
          if (millis() - startTime > 20000) {
            updateStatus("WiFi Timeout – versuche erneut...");
            WiFi.disconnect();
            WiFi.begin(ssid, password);
            startTime = millis();  // Timer zurücksetzen
          }
        }
      }
      break;

    case ST_GET_DATA:
      static bool done = false;

      if (!done) {
        fetchPrayerTimes();
        done = true;
      } else {
        currentStatus = ST_RUNNING;
      }
      break;

    case ST_RUNNING:
      changeScreen(SCREEN_HOME);
      break;
  }
}

void initAudio() {
  updateStatus("Audio Start...");

  if (!out) {
    out = new AudioOutputI2S();
  }
  out->SetPinout(32, 25, 21);
  // out->SetPinout(25, 26, 22);
  out->begin();
  out->SetGain(0.6);
}

void initSD() {
  updateStatus("SD Suche...");

  // SD AN
  initHardware(H_SD);

  hspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  bool ok = SD.begin(SD_CS, hspi, 1000000);
  sdCardOk = ok;
  
  if (ok) {
    updateStatus("SD OK!");
  } else {
    updateStatus("SD Karte konnte nicht gemountet werden.");  // Nicht hängen bleiben, einfach weitermachen
  }
  
  // SD AUS
  digitalWrite(SD_CS, HIGH);
}

void initWIFI() {
  updateStatus("Starte WiFi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

void updateStatus(const char* text) {

  if (ui_StartScreen_Label_StatusLabel == NULL) return;
  if (lastText == text) return;
  lastText = text;

  lv_label_set_text(ui_StartScreen_Label_StatusLabel, text);
  Serial.println(text);
}

void fetchPrayerTimes() {
  updateStatus("Daten holen!");

  HTTPClient http;
  http.begin(apiUrl);
  http.setTimeout(10000);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();

    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);

    prayers[0] = { "Fajr", doc["fajr"], timeToSeconds(doc["fajr"]) };
    prayers[1] = { "Duhur", doc["dohr"], timeToSeconds(doc["dohr"]) };
    prayers[2] = { "Asr", doc["asr"], timeToSeconds(doc["asr"]) };
    prayers[3] = { "Maghreb", doc["maghreb"], timeToSeconds(doc["maghreb"]) };
    prayers[4] = { "Isha", doc["icha"], timeToSeconds(doc["icha"]) };
  } else {
    Serial.println("HTTP Fehler!");
  }

  http.end();

  for (int i = 0; i < 5; i++) {
    Serial.printf("PRAYER %d: %s %s -> %ld\n",
                  i,
                  prayers[i].name.c_str(),
                  prayers[i].time.c_str(),
                  prayers[i].seconds);
  }

  updateStatus("Daten bereit!");
}

long timeToSeconds(String t) {
  int h = t.substring(0, 2).toInt();
  int m = t.substring(3, 5).toInt();
  return h * 3600 + m * 60;
}