#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/font.h"

#define OFFSET_X 29
#define OFFSET_Y 37

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(battery);
LV_IMG_DECLARE(battery_mask);

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    lv_draw_label_dsc_t outline_dsc;
    init_label_dsc(&outline_dsc, LVGL_BACKGROUND, &font, LV_TEXT_ALIGN_CENTER);
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &font, LV_TEXT_ALIGN_CENTER);

    #define BATT_OFFSET_Y 0
    #define BATT_PERIPHERAL_OFFSET_Y 36

    lv_canvas_draw_img(canvas, 0, BATT_OFFSET_Y, &battery, &img_dsc);
    lv_canvas_draw_rect(canvas, 4, 4 + BATT_OFFSET_Y, 54 * state->battery / 100, 23, &rect_dsc);
    lv_canvas_draw_img(canvas, 2, 2 + BATT_OFFSET_Y, &battery_mask, &img_dsc);

    lv_canvas_draw_img(canvas, 0, BATT_PERIPHERAL_OFFSET_Y, &battery, &img_dsc);
    if (state->peripheral_connected) {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_PERIPHERAL_OFFSET_Y, 54 * state->battery_peripheral / 100, 23, &rect_dsc);
    } else {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_PERIPHERAL_OFFSET_Y, 0, 23, &rect_dsc);
    }
    lv_canvas_draw_img(canvas, 2, 2 + BATT_PERIPHERAL_OFFSET_Y, &battery_mask, &img_dsc);

    char text[10] = {};
    sprintf(text, "%i%%", state->battery);
    char text_peripheral[10] = {};
    if (state->peripheral_connected) {
        sprintf(text_peripheral, "%i%%", state->battery_peripheral);
    } else {
        sprintf(text_peripheral, "?");
    }

    const int y = 9 + BATT_OFFSET_Y;
    const int w = 62;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                lv_canvas_draw_text(canvas, dx, y + dy, w, &outline_dsc, text);
            }
        }
    }

    lv_canvas_draw_text(canvas, 0, y, w, &label_dsc, text);

    const int y_peripheral = 9 + BATT_PERIPHERAL_OFFSET_Y;
    const int w_peripheral = 62;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                lv_canvas_draw_text(canvas, dx, y_peripheral + dy, w_peripheral, &outline_dsc, text_peripheral);
            }
        }
    }

    lv_canvas_draw_text(canvas, 0, y_peripheral, w_peripheral, &label_dsc, text_peripheral);

    // if (state->charging) {
    //     lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y, &bolt, &img_dsc);
    // }
}
