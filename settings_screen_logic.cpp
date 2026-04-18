#include "settings_screen_logic.h"
#include "app_state.h"
#include "ui.h"
#include <esp_system.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

// wichtig: Zugriff auf dein Audio
extern AudioOutputI2S* out;

static bool initialized = false;
static int currentVolume = 10;  // Startlautstärke, 0 bis 10
AudioGeneratorMP3* mp3_ton = nullptr;
AudioFileSourceSD* file_ton = nullptr;

// Forward declaration
static void go_to_home_screen(lv_event_t* e);
void reset_app(lv_event_t* e);
void volume_changed(lv_event_t* e);
void volume_increase(lv_event_t* e);
void volume_decrease(lv_event_t* e);
void theme_switch_event_handler(lv_event_t * e);

// -------------------------
// INIT
// -------------------------
void settings_screen_init() {
  if (initialized) return;


  if (ui_SettingsScreen_Button_ResetButton2 != NULL) {
    lv_obj_add_event_cb(ui_SettingsScreen_Button_ResetButton2, reset_app, LV_EVENT_CLICKED, NULL);
  }

  if (ui_SettingsScreen_Button_HomeButton != NULL) {
    lv_obj_add_event_cb(ui_SettingsScreen_Button_HomeButton, go_to_home_screen, LV_EVENT_CLICKED, NULL);
  }

  // Setup für die Plus-Taste (+)
  if (ui_SettingsScreen_Button_SoundPlusButton1 != NULL) {
    lv_obj_add_flag(ui_SettingsScreen_Button_SoundPlusButton1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_SettingsScreen_Button_SoundPlusButton1, volume_increase, LV_EVENT_CLICKED, NULL);
  }

  // Setup für die Minus-Taste (-)
  if (ui_SettingsScreen_Button_SoundMinusButton != NULL) {
    lv_obj_add_flag(ui_SettingsScreen_Button_SoundMinusButton, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_SettingsScreen_Button_SoundMinusButton, volume_decrease, LV_EVENT_CLICKED, NULL);
  }

  // Theme-Switch initialisieren
  if (ui_SettingsScreen_Switch_Theme != NULL) {
    lv_obj_add_flag(ui_SettingsScreen_Switch_Theme, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_clear_state(ui_SettingsScreen_Switch_Theme, LV_STATE_CHECKED);

    // Event-Handler für den Switch hinzufügen
    lv_obj_add_event_cb(ui_SettingsScreen_Switch_Theme, theme_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
  }


  // Lautstärkeanzeige zu Beginn aktualisieren
  update_volume_display();
  update_audio_gain();

  initialized = true;
}

// -------------------------
// Event-Handler für den Themen-Schalter
// -------------------------
void theme_switch_event_handler(lv_event_t * e)
{
    static bool current_theme_is_light = true; // Trackt den aktuellen Zustand (z. B. Light)

    lv_obj_t * obj = lv_event_get_target(e);
    bool is_checked = lv_obj_has_state(obj, LV_STATE_CHECKED);

    // Unterdrücken von wiederholten Events, wenn das Theme bereits aktiv ist
    if (is_checked == !current_theme_is_light) {
        return; // Kein Wechsel nötig
    }

    if (is_checked) {
        Serial.println("Theme set to: Dark");

        // Wechsel zu Dark Theme
        lv_theme_t * theme = lv_theme_default_init(
            NULL,
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_RED),
            true,                           // Dark Theme aktivieren
            LV_FONT_DEFAULT
        );
        lv_disp_set_theme(NULL, theme);   // Wende das Dark Theme auf das Display an
        current_theme_is_light = false;  // Aktualisiere Tracking-Variable
    } else {
        Serial.println("Theme set to: Light");

        // Wechsel zu Light Theme
        lv_theme_t * theme = lv_theme_default_init(
            NULL,
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_RED),
            false,                          // Light Theme aktivieren
            LV_FONT_DEFAULT
        );
        lv_disp_set_theme(NULL, theme);   // Wende das Light Theme auf das Display an
        current_theme_is_light = true;   // Aktualisiere Tracking-Variable
    }

    // Stile nach der Theme-Änderung aktualisieren
    lv_obj_invalidate(ui_SettingsScreen_Switch_Theme_Label);
    lv_obj_refresh_style(ui_Screen_SettingsScreen, LV_PART_ANY, LV_STYLE_PROP_INV);
}

// -------------------------
// RESET BUTTON CLICK EVENT
// -------------------------
void reset_app(lv_event_t* e) {
  Serial.printf("clicked reset_app\n");
  esp_restart();
}

// -------------------------
// LOOP
// -------------------------
void settings_screen_loop() {
  if (!initialized) {
    settings_screen_init();
  }

  // MP3-Ton abspielen
  handleToneTick();
}

// -------------------------
// 🔙 BACK TO HOME
// -------------------------
static void go_to_home_screen(lv_event_t* e) {
  changeScreen(SCREEN_HOME);
}

// Lautstärke erhöhen
void volume_increase(lv_event_t* e) {
  // Wenn der Wert weniger als 10 ist, erhöhe ihn
  if (currentVolume < 10) {
    currentVolume++;          // Lautstärkewert erhöhen
    update_volume_display();  // Anzeige aktualisieren
    update_audio_gain();      // Audio-Gain aktualisieren
    playTone();
  }
}

// Lautstärke verringern
void volume_decrease(lv_event_t* e) {
  // Wenn der Wert mehr als 0 ist, verringere ihn
  if (currentVolume > 0) {
    currentVolume--;          // Lautstärkewert verringern
    update_volume_display();  // Anzeige aktualisieren
    update_audio_gain();      // Audio-Gain aktualisieren
    playTone();
  }
}

// Lautstärkeanzeige aktualisieren
void update_volume_display() {
  // Lautstärkewert auf dem Label anzeigen
  if (ui_SettingsScreen_Label_SoundLevelLabel != NULL) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%d", currentVolume);
    lv_label_set_text(ui_SettingsScreen_Label_SoundLevelLabel, buf);
    lv_obj_invalidate(ui_SettingsScreen_Label_SoundLevelLabel);  // Anzeige aktualisieren
  }
}

// Audio-Gain aktualisieren
void update_audio_gain() {
  // Gain-Wert berechnen (0 bis 10 auf 0.0 bis 1.0)
  float gain = currentVolume / 10.0;

  if (out != NULL) {
    out->SetGain(gain);  // Lautstärke im Audio-Ausgang setzen
  }

  // Debug-Ausgabe in die Konsole
  Serial.printf("Volume: %d (Gain: %.2f)\n", currentVolume, gain);
}

void handleToneTick() {
  if (mp3Tone && mp3Tone->isRunning()) {
    if (!mp3Tone->loop()) {
      mp3Tone->stop();
      delete mp3Tone;
      mp3Tone = nullptr;

      delete toneFile;
      toneFile = nullptr;
    }
  }
}

void playTone() {
  // Wenn schon ein Ton läuft, stoppen und freigeben
  if (mp3Tone) {
    mp3Tone->stop();
    delete mp3Tone;
    mp3Tone = nullptr;
  }
  if (toneFile) {
    delete toneFile;
    toneFile = nullptr;
  }

  // Neues MP3-File laden
  toneFile = new AudioFileSourceSD("/beep.mp3");
  mp3Tone = new AudioGeneratorMP3();

  if (!mp3Tone->begin(toneFile, out)) {
    Serial.println("MP3 Tone start failed!");
    delete mp3Tone;
    mp3Tone = nullptr;
    delete toneFile;
    toneFile = nullptr;
    return;
  }

  // Gain sofort setzen
  if (out) {
    float gain = currentVolume / 10.0;
    out->SetGain(gain);
  }

  Serial.println("Tone gestartet!");
}
