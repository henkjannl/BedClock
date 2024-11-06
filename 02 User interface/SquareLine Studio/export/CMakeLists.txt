SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_open_circle_png.c
    images/ui_img_closed_circle_png.c)

add_library(ui ${SOURCES})
