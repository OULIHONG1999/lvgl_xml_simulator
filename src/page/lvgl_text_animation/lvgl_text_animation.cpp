//
// Created by Administrator on 2025/5/24.
//

#include "lvgl_text_animation.h"
#include "lvgl_text_animation.h"
#include <cstring>

// ================== TextTyper 实现 ==================
TextTyper::TextTyper(lv_obj_t* parent, const lv_font_t* font) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, font, 0);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, 200); // 默认宽度
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
}

TextTyper::~TextTyper() {
    if (typeTimer) lv_timer_del(typeTimer);
    if (fadeTimer) lv_timer_del(fadeTimer);
    lv_obj_del(label);
}

void TextTyper::start(const char* text, uint32_t speedMsPerChar) {
    if (isTyping) return;

    currentText = text;
    charIndex = 0;
    typeSpeed = speedMsPerChar;
    charAppearTimes.clear();
    charAppearTimes.resize(currentText.length(), 0);
    lv_label_set_text(label, "");

    isTyping = true;
    typeTimer = lv_timer_create([](lv_timer_t* t) {
        TextTyper* self = reinterpret_cast<TextTyper*>(t->user_data);
        if (self->charIndex < self->currentText.length()) {
            // 记录字符出现时间
            if (self->enableFadeIn) {
                self->charAppearTimes[self->charIndex] = lv_tick_get();
            }
            self->charIndex++;
            self->updateFadeText();
        } else {
            self->isTyping = false;
            lv_timer_del(t);
            if (self->onFinishedCallback) self->onFinishedCallback();
        }
    }, typeSpeed, this);

    if (enableFadeIn) {
        fadeTimer = lv_timer_create([](lv_timer_t* t) {
            TextTyper* self = reinterpret_cast<TextTyper*>(t->user_data);
            self->updateFadeText();
        }, 16, this); // 60Hz更新透明度
    }
}

void TextTyper::pause() {
    if (typeTimer) lv_timer_pause(typeTimer);
    if (fadeTimer) lv_timer_pause(fadeTimer);
}

void TextTyper::resume() {
    if (typeTimer) lv_timer_resume(typeTimer);
    if (fadeTimer) lv_timer_resume(fadeTimer);
}

void TextTyper::enableFadeEffect(bool enable, uint32_t durationMs) {
    enableFadeIn = enable;
    fadeDuration = durationMs;
    if (enable) {
        lv_obj_add_flag(label, LV_OBJ_FLAG_ADV_HITTEST); // 启用高级渲染
    }
}

uint8_t TextTyper::calculateOpacity(size_t idx) const {
    if (!enableFadeIn || idx >= charAppearTimes.size()) return 255;
    uint32_t elapsed = lv_tick_get() - charAppearTimes[idx];
    return elapsed > fadeDuration ? 255 : (uint8_t)(255 * elapsed / fadeDuration);
}

void TextTyper::updateFadeText() {
    if (!enableFadeIn) {
        lv_label_set_text(label, currentText.substr(0, charIndex).c_str());
        return;
    }

    std::string richText = "";
    for (size_t i = 0; i < charIndex; i++) {
        uint8_t opacity = calculateOpacity(i);
        richText += "#" + std::to_string(opacity) + "ffffff" + currentText[i]; // 白色文本
    }
    lv_label_set_text(label, richText.c_str());
}

// ================== TextContainer 实现 ==================
TextContainer::TextContainer(lv_obj_t* parent, lv_coord_t width, lv_coord_t initialHeight) {
    container = lv_obj_create(parent);
    lv_obj_set_size(container, width, initialHeight);
    lv_obj_set_style_bg_color(container, lv_color_white(), 0);
    lv_obj_set_style_border_width(container, 1, 0);
}

void TextContainer::updateHeight(lv_coord_t newHeight) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, container);
    lv_anim_set_values(&a, lv_obj_get_height(container), newHeight);
    lv_anim_set_time(&a, 200);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

// ================== ScrollManager 实现 ==================
ScrollManager::ScrollManager(lv_obj_t* container) : container(container) {
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
}

void ScrollManager::attachContent(lv_obj_t* content) {
    this->content = content;
    lv_obj_set_parent(content, container);
}

void ScrollManager::autoScrollToBottom() {
    contentHeight = lv_obj_get_height(content);
    if (contentHeight <= lv_obj_get_height(container)) return;

    lv_coord_t targetY = lv_obj_get_height(container) - contentHeight;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, content);
    lv_anim_set_values(&a, lv_obj_get_y(content), targetY);
    lv_anim_set_time(&a, 200);
    lv_anim_start(&a);
}

void ScrollManager::startAutoScroll() {
    stopScroll();
    autoScrollToBottom();
    isAutoScroll = true;
}

void ScrollManager::startLoopScroll(int speedPxPerMs) {
    stopScroll();
    contentHeight = lv_obj_get_height(content);
    if (contentHeight <= lv_obj_get_height(container)) return;

    // 创建克隆对象
    cloneContent = lv_label_create(container);
    lv_obj_set_style_text_font(cloneContent, lv_obj_get_style_text_font(content, 0), 0);
    lv_label_set_text(cloneContent, lv_label_get_text(content));
    lv_obj_set_width(cloneContent, lv_obj_get_width(content));
    lv_obj_set_y(cloneContent, lv_obj_get_height(container));

    scrollSpeed = speedPxPerMs;
    scrollTimer = lv_timer_create(loopScrollCallback, 10, this);
    isLoopScroll = true;
}

void ScrollManager::loopScrollCallback(lv_timer_t* timer) {
    ScrollManager* self = reinterpret_cast<ScrollManager*>(timer->user_data);
    lv_coord_t y = lv_obj_get_y(self->content);
    lv_coord_t cloneY = lv_obj_get_y(self->cloneContent);

    y -= self->scrollSpeed;
    cloneY -= self->scrollSpeed;

    lv_obj_set_y(self->content, y);
    lv_obj_set_y(self->cloneContent, cloneY);

    // 循环逻辑
    if (y <= -self->contentHeight) {
        lv_obj_set_y(self->content, cloneY + self->contentHeight);
    }
    if (cloneY <= -self->contentHeight) {
        lv_obj_set_y(self->cloneContent, y + self->contentHeight);
    }
}

void ScrollManager::stopScroll() {
    if (scrollTimer) {
        lv_timer_del(scrollTimer);
        scrollTimer = nullptr;
    }
    if (cloneContent) {
        lv_obj_del(cloneContent);
        cloneContent = nullptr;
    }
    isAutoScroll = isLoopScroll = false;
}

void ScrollManager::resetPosition() {
    stopScroll();
    lv_obj_set_y(content, 10);
    if (cloneContent) lv_obj_set_y(cloneContent, lv_obj_get_height(container));
}