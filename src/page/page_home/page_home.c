//
// Created by Administrator on 2025/4/16.
//

#include "page_home.h"
#include "../../page_manager/page_manager.h"
#include "stdio.h"


static bool create_activity();

static void destroy_activity(unsigned char en_clean);

static void init_indev_group();

user_activity_t act_home = {
    .activity_obj = NULL,
    .create_activity = create_activity, // 请替换为实际函数指针
    .destroy_activity = destroy_activity,
    .param = NULL
};

#define ACTIVITY (act_home.activity_obj)
#define OBJ lv_obj_t
static lv_group_t *indev_group = NULL;


static lv_img_dsc_t image = {0};

typedef struct {
    lv_obj_t *page_bg;

    lv_obj_t *label_qmi8658c;
    lv_obj_t *label_gxhtc3_temp;
    lv_obj_t *label_gxhtc3_humi;

    // 圆
    lv_obj_t *img_circle;
    lv_obj_t *key_led;
} atc_home_page_t;

static atc_home_page_t hp;
// MAP的宏定义
#define MAP(x, in_min, in_max, out_min, out_max) (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

static void set_circle_pos(int val_x, int val_y) {
    // 计算圆的位置
    lv_coord_t x = MAP(val_x, -90, 90, 0, 240);
    lv_coord_t y = MAP(val_y, -90, 90, 0, 240);
    // ESP_LOGI("set_circle_pos", "x:%d, y:%d", x, y);
    if (hp.img_circle != NULL) {
        lv_obj_set_pos(hp.img_circle, x, y);
    }
}



static void create_act_home_page(lv_obj_t *parent) {
    lv_obj_t *page_bg = lv_obj_create(parent);
    lv_obj_set_size(page_bg, 240, 240);
    lv_obj_set_style_bg_color(page_bg, lv_color_hex(0x5286F9), 0);
    lv_obj_set_style_radius(page_bg, 0, 0);

    lv_obj_t *label_qmi8658c = lv_label_create(page_bg);
    lv_obj_set_style_text_color(label_qmi8658c, lv_color_hex(0xFFFFFF), 0);
    // 文本居中
    lv_obj_set_style_text_align(label_qmi8658c, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(label_qmi8658c, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(label_qmi8658c, "QMI8658C");

    lv_obj_t *label_gxhtc3_temp = lv_label_create(page_bg);
    lv_obj_set_style_text_color(label_gxhtc3_temp, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_gxhtc3_temp, LV_ALIGN_TOP_MID, 0, 90);
    lv_label_set_text(label_gxhtc3_temp, "TEMP:0.0");

    lv_obj_t *label_gxhtc3_humi = lv_label_create(page_bg);
    lv_obj_set_style_text_color(label_gxhtc3_humi, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_gxhtc3_humi, LV_ALIGN_TOP_MID, 0, 120);
    lv_label_set_text(label_gxhtc3_humi, "HUMI:0.0");

    // 圆
    lv_obj_t *led = lv_led_create(page_bg);
    // lv_obj_align(led, LV_ALIGN_CENTER, 0, 150);
    lv_led_on(led);
    lv_led_set_color(led, lv_palette_main(LV_PALETTE_RED));

    lv_obj_t *key_led = lv_led_create(page_bg);
    lv_obj_set_pos(key_led, 0, 0);
    lv_led_on(key_led);
    lv_led_set_color(key_led, lv_palette_main(LV_PALETTE_PURPLE));


    hp.page_bg = page_bg;
    hp.label_qmi8658c = label_qmi8658c;
    hp.label_gxhtc3_temp = label_gxhtc3_temp;
    hp.label_gxhtc3_humi = label_gxhtc3_humi;
    hp.img_circle = led;
    hp.key_led = key_led;
}

static void set_key_led(const lv_coord_t x, const lv_coord_t y) {
    lv_obj_set_pos(hp.key_led, x, y);
}



static bool create_activity() {
    init_indev_group();

    if (ACTIVITY == NULL) {
        ACTIVITY = lv_obj_create(NULL);
        lv_obj_set_size(ACTIVITY, 240, 240);
        lv_obj_set_style_bg_color(ACTIVITY, lv_color_hex(0x5286F9), 0);
        create_act_home_page(ACTIVITY);
    }
    lv_scr_load_anim(ACTIVITY, LV_SCR_LOAD_ANIM_FADE_OUT, 200, 10, false);

    return true;
}

static lv_obj_t *focus_obj_last = NULL;

static void init_indev_group() {
    // 将indev group绑定到默认的group上
    if (indev_group == NULL) {
        indev_group = lv_group_get_default();
    } else {
        lv_group_focus_obj(focus_obj_last);
    }
}

void destroy_res() {
}

static void destroy_activity(unsigned char en_clean) {
    printf("destroy rbt_page_boot page %p\n", ACTIVITY);
    if (en_clean) {
        destroy_res();
    } else {
    }
}
