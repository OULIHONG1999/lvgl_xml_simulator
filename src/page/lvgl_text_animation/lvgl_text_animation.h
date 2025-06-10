#ifndef LVGL_TEXT_ANIMATION_H
#define LVGL_TEXT_ANIMATION_H

#include "lvgl/lvgl.h"
#include <string>
#include <vector>
#include <functional>


// LvTextAnimation
class LvTextAnimation {
    // 实现文本逐字播放效果
    // 基于定时器定时增加需要显示的文字
private:
    // 目标显示的文本
    std::string target_text;
    // 当前显示的文本
    std::string current_text;
    // 显示到的字符索引
    int current_index = 0;

    lv_timer_t *timer = nullptr;
    lv_obj_t *label = nullptr;

public:
    //  构造函数
    LvTextAnimation(lv_obj_t *parent, const lv_font_t *font, uint32_t anim_speed = 100);

    // 析构函数
    ~LvTextAnimation();

    // 定时器回调函数
    static void timer_callback(lv_timer_t *timer);

    // 设置要显示的文本
    void set_text(const std::string &text);

    // 设置文本动画速度
    void set_anim_speed(uint32_t anim_speed);

    // 启动动画
    void start();

    // 暂停动画
    void pause();

    // 停止动画
    void stop();
};


#endif // LVGL_TEXT_ANIMATION_H
