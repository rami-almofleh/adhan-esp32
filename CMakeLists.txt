SET(SOURCES ui_comp_systemloadinglabel.c
    ui_comp.c
    ui_Screen_StartScreen.c
    ui_Screen_HomeScreen.c
    ui_Screen_PrayersScreen.c
    ui_Screen_SettingsScreen.c
    ui_Screen_AdhanSelectScreen.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    home_screen_ui.cpp
    settings_screen_ui.cpp
    prayers_screen_ui.cpp
    azan_screen_ui.cpp
    adhan_select_screen_ui.cpp)

add_library(ui ${SOURCES})
