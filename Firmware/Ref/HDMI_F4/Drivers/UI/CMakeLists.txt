SET(SOURCES components/ui_comp_container1.c
    components/ui_comp.c
    screens/ui_Main.c
    screens/ui_Setting.c
    screens/ui_Run.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_left_png.c
    images/ui_img_seting_png.c
    images/ui_img_power_png.c
    images/ui_img_powered_png.c
    images/ui_img_back_png.c
    images/ui_img_backed_png.c
    images/ui_img_play_png.c
    images/ui_img_pause_png.c
    images/ui_img_back_run_png.c
    images/ui_img_back1_png.c
    fonts/ui_font_Font1.c
    fonts/ui_font_Light.c
    fonts/ui_font_Light40.c)

add_library(ui ${SOURCES})
