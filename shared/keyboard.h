#ifndef BBX_KEYBOARD_H
#define BBX_KEYBOARD_H

#include "lvgl/lvgl.h"
#include <stdbool.h>

typedef struct {
    lv_keyboard_mode_t mode;
    int layout_id;
    bool popovers;
    int32_t height;
    lv_event_cb_t value_changed_callback;
    lv_event_cb_t ready_callback;
} bbx_keyboard_config_t;

lv_obj_t *bbx_keyboard_create(lv_obj_t *parent, lv_obj_t *textarea, bbx_keyboard_config_t *config);
void bbx_keyboard_init_config(bbx_keyboard_config_t *config);

#endif /* BBX_KEYBOARD_H */
