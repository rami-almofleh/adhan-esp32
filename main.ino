#include <TFT_eSPI.h>
#include <lvgl.h>
#include <ui.h>
#include <XPT2046_Touchscreen.h>

#include "app_state.h"
#include "start_screen_logic.h"
#include "home_screen_logic.h"
#include "prayers_screen_logic.h"
#include "settings_screen_logic.h"
#include "azan_screen_logic.h"
#include "adhan_select_screen_logic.h"

TFT_eSPI tft = TFT_eSPI();

// Display Größe
static const uint16_t DISP_HOR_RES = 320;
static const uint16_t DISP_VER_RES = 240;

// LVGL Buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISP_HOR_RES * 20];

// TOUCH
#define CS_PIN 33                // Touch Chip Select
XPT2046_Touchscreen ts(CS_PIN);  // global verfügbar
#define SD_CS 5

// =====================
// DISPLAY FLUSH
// =====================
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {

  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  // TFT AN
  initHardware(H_TFT);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)color_p, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// =====================
// INIT DISPLAY DRIVER
// =====================
void init_disp_driver() {
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISP_HOR_RES * 20);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.full_refresh = 1;

  lv_disp_drv_register(&disp_drv);
}

// =====================
// TOUCH READ
// =====================
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  // TOUCH AN
  initHardware(H_TOUCH);

  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    
    if (p.z > 200) {
      data->point.x = map(p.x, 200, 3800, DISP_HOR_RES, 0);
      data->point.y = map(p.y, 200, 3800, 0, DISP_VER_RES);
      data->state = LV_INDEV_STATE_PR;
    } else {
      data->state = LV_INDEV_STATE_REL;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }

  digitalWrite(TOUCH_CS, HIGH);
}

void updateTime() {
  time_t now;
  time(&now);

  if (now < 100000) {
    timeValid = false;
    return;
  }

  localtime_r(&now, &globalTime);
  timeValid = true;
}

// =====================
// SETUP
// =====================
void setup() {
  Serial.begin(115200);

  // SPI
  SPI.begin(14, 12, 13);
  // SPI.begin(18, 19, 23);

  // CS Pins
  pinMode(TFT_CS, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(SD_CS, OUTPUT);

  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TOUCH_CS, HIGH);

  ts.begin(SPI);
  ts.setRotation(1);

  // TFT
  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.setSwapBytes(true);

  // LVGL
  lv_init();
  init_disp_driver();

  load_settings();
  ui_init();
  apply_settings();

  home_screen_init();
  prayers_screen_init();
  settings_screen_init();
  azan_screen_init();
  adhan_select_screen_init();

  start_screen_init();

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

void loop() {
  // Update Time
  static unsigned long lastTimeUpdate = 0;

  if (millis() - lastTimeUpdate >= 1000) {
    lastTimeUpdate = millis();
    updateTime();
    updateStatusIcons();
    if (currentStatus == ST_RUNNING) {
      checkAdhanLogic();
    }
  }

  switch (currentScreen) {
    case SCREEN_START:
      start_screen_loop();
      break;
    case SCREEN_HOME:
      home_screen_loop();
      break;
    case SCREEN_PRAYERS:
      prayers_screen_loop();
      break;
    case SCREEN_SETTINGS:
      settings_screen_loop();
      break;
    case SCREEN_AZAN:
      azan_screen_loop();
      break;
    case SCREEN_ADHAN_SELECT:
      adhan_select_screen_loop();
      break;
  }

  lv_tick_inc(5);
  lv_timer_handler();
  delay(5);
}
