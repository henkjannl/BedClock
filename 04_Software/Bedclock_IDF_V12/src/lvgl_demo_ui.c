/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "lvgl.h"
#include "ui_img_labels_png.c"
#include "ui_img_closed_circle_png.c"
#include "ui_img_open_circle_png.c"

void movedown_Animation(lv_obj_t * TargetObject, int delay);

lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel;
lv_obj_t * ui_lbl_intensity;
lv_obj_t * ui_img_intensity_00;
lv_obj_t * ui_img_intensity_01;
lv_obj_t * ui_img_intensity_02;
lv_obj_t * ui_img_intensity_03;
lv_obj_t * ui_img_intensity_04;
lv_obj_t * ui_lbl_color;
lv_obj_t * ui_img_color_00;
lv_obj_t * ui_img_color_01;
lv_obj_t * ui_img_color_02;
lv_obj_t * ui_img_color_03;
lv_obj_t * ui_lbl_timer;
lv_obj_t * ui_img_timer_00;
lv_obj_t * ui_img_timer_01;
lv_obj_t * ui_img_timer_02;
lv_obj_t * ui_img_timer_03;
lv_obj_t * ui_img_timer_04;
lv_obj_t * ui_lbl_display;
lv_obj_t * ui_img_display_01;
lv_obj_t * ui_img_display_02;
lv_obj_t * ui_img_display_03;

void animation_demo(lv_disp_t *disp) {
    ui_Screen1 = lv_disp_get_scr_act(disp);

    ui_img_timer_04 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_img_timer_04, &ui_img_closed_circle_png);
    lv_obj_set_width(ui_img_timer_04, 10);
    lv_obj_set_height(ui_img_timer_04, 16);
    lv_obj_set_x(ui_img_timer_04, 10);
    lv_obj_set_y(ui_img_timer_04,  0);
    lv_obj_add_flag(ui_img_timer_04, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_04, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // movedown_Animation(ui_img_timer_04, 0);
}

void example_lvgl_demo_ui(lv_disp_t *disp)
{
    ui_Screen1 = lv_disp_get_scr_act(disp);

    ui_Panel = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_Panel);
    lv_obj_set_width(ui_Panel, 128);
    lv_obj_set_height(ui_Panel, 128);
    lv_obj_set_x(ui_Panel,  0);
    lv_obj_set_y(ui_Panel, -64);
    lv_obj_clear_flag(ui_Panel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lbl_intensity = lv_label_create(ui_Panel);
    lv_obj_set_width(ui_lbl_intensity, 78);
    lv_obj_set_height(ui_lbl_intensity, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lbl_intensity, 0);
    lv_obj_set_y(ui_lbl_intensity, 64);
    lv_label_set_long_mode(ui_lbl_intensity, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_lbl_intensity, "Intensity");
    lv_obj_set_style_text_font(ui_lbl_intensity, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_img_intensity_00 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_intensity_00, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_intensity_00, 10);
    lv_obj_set_height(ui_img_intensity_00, 16);
    lv_obj_set_x(ui_img_intensity_00, 70);
    lv_obj_set_y(ui_img_intensity_00, 64);
    lv_obj_add_flag(ui_img_intensity_00, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_intensity_00, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_intensity_01 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_intensity_01, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_intensity_01, 10);
    lv_obj_set_height(ui_img_intensity_01, 16);
    lv_obj_set_x(ui_img_intensity_01, 80);
    lv_obj_set_y(ui_img_intensity_01, 64);
    lv_obj_add_flag(ui_img_intensity_01, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_intensity_01, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_intensity_02 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_intensity_02, &ui_img_closed_circle_png);
    lv_obj_set_width(ui_img_intensity_02, 10);
    lv_obj_set_height(ui_img_intensity_02, 16);
    lv_obj_set_x(ui_img_intensity_02, 90);
    lv_obj_set_y(ui_img_intensity_02, 64);
    lv_obj_add_flag(ui_img_intensity_02, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_intensity_02, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_intensity_03 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_intensity_03, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_intensity_03, 10);
    lv_obj_set_height(ui_img_intensity_03, 16);
    lv_obj_set_x(ui_img_intensity_03, 100);
    lv_obj_set_y(ui_img_intensity_03, 64);
    lv_obj_add_flag(ui_img_intensity_03, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_intensity_03, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_intensity_04 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_intensity_04, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_intensity_04, 10);
    lv_obj_set_height(ui_img_intensity_04, 16);
    lv_obj_set_x(ui_img_intensity_04, 110);
    lv_obj_set_y(ui_img_intensity_04, 64);
    lv_obj_add_flag(ui_img_intensity_04, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_intensity_04, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lbl_color = lv_label_create(ui_Panel);
    lv_obj_set_width(ui_lbl_color, 78);
    lv_obj_set_height(ui_lbl_color, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lbl_color, 0);
    lv_obj_set_y(ui_lbl_color, 80);
    lv_label_set_text(ui_lbl_color, "Color");
    lv_obj_set_style_text_font(ui_lbl_color, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_img_color_00 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_color_00, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_color_00, 10);
    lv_obj_set_height(ui_img_color_00, 16);
    lv_obj_set_x(ui_img_color_00, 70);
    lv_obj_set_y(ui_img_color_00, 80);
    lv_obj_add_flag(ui_img_color_00, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_color_00, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_color_01 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_color_01, &ui_img_closed_circle_png);
    lv_obj_set_width(ui_img_color_01, 10);
    lv_obj_set_height(ui_img_color_01, 16);
    lv_obj_set_x(ui_img_color_01, 80);
    lv_obj_set_y(ui_img_color_01, 80);
    lv_obj_add_flag(ui_img_color_01, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_color_01, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_color_02 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_color_02, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_color_02, 10);
    lv_obj_set_height(ui_img_color_02, 16);
    lv_obj_set_x(ui_img_color_02, 90);
    lv_obj_set_y(ui_img_color_02, 80);
    lv_obj_add_flag(ui_img_color_02, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_color_02, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_color_03 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_color_03, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_color_03, 10);
    lv_obj_set_height(ui_img_color_03, 16);
    lv_obj_set_x(ui_img_color_03, 100);
    lv_obj_set_y(ui_img_color_03, 80);
    lv_obj_add_flag(ui_img_color_03, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_color_03, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lbl_timer = lv_label_create(ui_Panel);
    lv_obj_set_width(ui_lbl_timer, 78);
    lv_obj_set_height(ui_lbl_timer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lbl_timer, 0);
    lv_obj_set_y(ui_lbl_timer, 96);
    lv_label_set_text(ui_lbl_timer, "Timer");
    lv_obj_set_style_text_font(ui_lbl_timer, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_img_timer_00 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_timer_00, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_timer_00, 10);
    lv_obj_set_height(ui_img_timer_00, 16);
    lv_obj_set_x(ui_img_timer_00, 70);
    lv_obj_set_y(ui_img_timer_00, 96);
    lv_obj_add_flag(ui_img_timer_00, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_00, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_timer_01 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_timer_01, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_timer_01, 10);
    lv_obj_set_height(ui_img_timer_01, 16);
    lv_obj_set_x(ui_img_timer_01, 80);
    lv_obj_set_y(ui_img_timer_01, 96);
    lv_obj_add_flag(ui_img_timer_01, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_01, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_timer_02 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_timer_02, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_timer_02, 10);
    lv_obj_set_height(ui_img_timer_02, 16);
    lv_obj_set_x(ui_img_timer_02, 90);
    lv_obj_set_y(ui_img_timer_02, 96);
    lv_obj_add_flag(ui_img_timer_02, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_02, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_timer_03 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_timer_03, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_timer_03, 10);
    lv_obj_set_height(ui_img_timer_03, 16);
    lv_obj_set_x(ui_img_timer_03, 100);
    lv_obj_set_y(ui_img_timer_03, 96);
    lv_obj_add_flag(ui_img_timer_03, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_03, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_timer_04 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_timer_04, &ui_img_closed_circle_png);
    lv_obj_set_width(ui_img_timer_04, 10);
    lv_obj_set_height(ui_img_timer_04, 16);
    lv_obj_set_x(ui_img_timer_04, 110);
    lv_obj_set_y(ui_img_timer_04, 96);
    lv_obj_add_flag(ui_img_timer_04, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_timer_04, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lbl_display = lv_label_create(ui_Panel);
    lv_obj_set_width(ui_lbl_display, 78);
    lv_obj_set_height(ui_lbl_display, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lbl_display, 0);
    lv_obj_set_y(ui_lbl_display, 112);
    lv_label_set_text(ui_lbl_display, "Display");
    lv_obj_set_style_text_font(ui_lbl_display, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_img_display_01 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_display_01, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_display_01, 10);
    lv_obj_set_height(ui_img_display_01, 16);
    lv_obj_set_x(ui_img_display_01, 70);
    lv_obj_set_y(ui_img_display_01, 112);
    lv_obj_add_flag(ui_img_display_01, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_display_01, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_display_02 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_display_02, &ui_img_open_circle_png);
    lv_obj_set_width(ui_img_display_02, 10);
    lv_obj_set_height(ui_img_display_02, 16);
    lv_obj_set_x(ui_img_display_02, 80);
    lv_obj_set_y(ui_img_display_02, 112);
    lv_obj_add_flag(ui_img_display_02, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_display_02, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_img_display_03 = lv_img_create(ui_Panel);
    lv_img_set_src(ui_img_display_03, &ui_img_closed_circle_png);
    lv_obj_set_width(ui_img_display_03, 10);
    lv_obj_set_height(ui_img_display_03, 16);
    lv_obj_set_x(ui_img_display_03, 90);
    lv_obj_set_y(ui_img_display_03, 112);
    lv_obj_add_flag(ui_img_display_03, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_display_03, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // movedown_Animation(ui_Panel, 0);
}

typedef struct _ui_anim_user_data_t {
    lv_obj_t * target;
    lv_img_dsc_t ** imgset;
    int32_t imgset_size;
    int32_t val;
} ui_anim_user_data_t;

void _ui_anim_callback_set_x(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_x(usr->target, v);
}


void _ui_anim_callback_set_y(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_y(usr->target, v);
}

void _ui_anim_callback_set_width(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_width(usr->target, v);
}


void _ui_anim_callback_set_height(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_height(usr->target, v);
}

void _ui_anim_callback_set_opacity(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_style_opa(usr->target, v, 0);
}

void _ui_anim_callback_set_image_zoom(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_img_set_zoom(usr->target, v);
}

void _ui_anim_callback_set_image_angle(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_img_set_angle(usr->target, v);
}

void _ui_anim_callback_set_image_frame(lv_anim_t * a, int32_t v) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    usr->val = v;

    if(v < 0) v = 0;
    if(v >= usr->imgset_size) v = usr->imgset_size - 1;
    lv_img_set_src(usr->target, usr->imgset[v]);
}

int32_t _ui_anim_callback_get_x(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_x_aligned(usr->target);
}

int32_t _ui_anim_callback_get_y(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_y_aligned(usr->target);
}

int32_t _ui_anim_callback_get_width(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_width(usr->target);
}

int32_t _ui_anim_callback_get_height(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_height(usr->target);
}

int32_t _ui_anim_callback_get_opacity(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_style_opa(usr->target, 0);
}

int32_t _ui_anim_callback_get_image_zoom(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_img_get_zoom(usr->target);
}

int32_t _ui_anim_callback_get_image_angle(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_img_get_angle(usr->target);
}

int32_t _ui_anim_callback_get_image_frame(lv_anim_t * a) {
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return usr->val;
}

void _ui_anim_callback_free_user_data(lv_anim_t * a) {
    lv_mem_free(a->user_data);
    a->user_data = NULL;
}

void movedown_Animation(lv_obj_t * TargetObject, int delay) {
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = 10;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 10, 20);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
    lv_anim_start(&PropertyAnimation_0);
}