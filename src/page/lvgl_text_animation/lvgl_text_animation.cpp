#include "lvgl_text_animation.h"


LvTextAnimation::LvTextAnimation(lv_obj_t *parent, const lv_font_t *font, uint32_t anim_speed) {
    label = lv_label_create(parent);
    if (font != nullptr) {
        lv_obj_set_style_text_font(label, font, 0);
    }
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, "Default Text");

    printf("LvTextAnimation this: %p\n", this);
    // 创建定时器
    timer = lv_timer_create(timer_callback, anim_speed, this);
    lv_timer_pause(timer);
}

LvTextAnimation::~LvTextAnimation() = default;

void LvTextAnimation::timer_callback(lv_timer_t *timer) {
    auto *self = static_cast<LvTextAnimation *>(timer->user_data);
    if (self->current_index < self->target_text.length()) {
        lv_label_ins_text(self->label,
            LV_LABEL_POS_LAST,
            self->target_text.substr(self->current_index, 1).c_str());
        self->current_index++;
    }
}

void LvTextAnimation::set_text(const std::string &text) {
    printf("Setting text: %s\n", text.c_str());

    // 暂停定时器
    lv_timer_pause(timer);
    target_text = text;
    current_text.clear();
    current_index = 0;
    if (label) {
        lv_label_set_text(label, "");
    }
    if (timer) {
        lv_timer_resume(timer);
    }
}

void LvTextAnimation::set_anim_speed(uint32_t anim_speed) {
}

void LvTextAnimation::start() {
    lv_timer_resume(timer);
    lv_label_set_text(label, "");
    current_index = 0;
}

void LvTextAnimation::pause() {
}

void LvTextAnimation::stop() {
}


// 测试函数
void test_lvgl_text_animation() {
    lv_obj_t *parent = lv_obj_create(lv_scr_act());
    lv_obj_set_size(parent, 200, 200);
    lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);

    static LvTextAnimation text_anim(parent, nullptr, 100);
    text_anim.set_text("Hello, World!");
    text_anim.start();
}
