//
// Created by Administrator on 2025/5/24.
//

#include "LvChatAnimation.h"
#include <cstring>

LvChatAnimation::LvChatAnimation(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                                 const lv_font_t *font, uint32_t typeSpeed)
    : width(width), height(height), charIndex(0), typeSpeed(typeSpeed),
      currentState(WAITING), enableInfiniteScroll(true), isCloning(false),
      onCharacterTyped(nullptr) {
    // 创建容器
    container = lv_obj_create(parent);
    lv_obj_set_size(container, width, height);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // 初始化文本样式
    lv_style_init(&textStyle);
    lv_style_set_text_font(&textStyle, font);
    lv_style_set_text_color(&textStyle, lv_color_black());

    // 创建文本区域
    textArea = lv_label_create(container);
    lv_obj_add_style(textArea, &textStyle, 0);
    lv_label_set_long_mode(textArea, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(textArea, width - 20); // 留出边距
    lv_obj_align(textArea, LV_ALIGN_TOP_LEFT, 10, 10);


    // 初始化克隆文本区域（用于无限滚动）
    cloneTextArea = nullptr;

    // 创建定时器
    typeTimer = lv_timer_create(typeCallback, typeSpeed, this);
    lv_timer_pause(typeTimer);

    scrollTimer = lv_timer_create(scrollCallback, 10, this);
    lv_timer_pause(scrollTimer);
}

LvChatAnimation::~LvChatAnimation() {
    if (typeTimer) lv_timer_del(typeTimer);
    if (scrollTimer) lv_timer_del(scrollTimer);
    if (textArea) lv_obj_del(textArea);
    if (cloneTextArea) lv_obj_del(cloneTextArea);
    if (container) lv_obj_del(container);
    lv_style_reset(&textStyle);
}

void LvChatAnimation::typeCallback(lv_timer_t *timer) {
    LvChatAnimation *anim = static_cast<LvChatAnimation *>(timer->user_data);

    // 逐字显示逻辑
    if (anim->charIndex < anim->currentMessage.length()) {
        char c = anim->currentMessage[anim->charIndex++];
        anim->updateTextDisplay();

        // 调用语音回调
        if (anim->onCharacterTyped) {
            anim->onCharacterTyped(c);
        }

        // 检查是否需要滚动
        anim->checkAndScroll();
    } else {
        // 当前消息显示完毕
        lv_timer_pause(timer);
        anim->currentState = WAITING;

        // 如果启用了无限滚动，开始滚动
        if (anim->enableInfiniteScroll) {
            anim->startInfiniteScroll();
        }
    }
}

void LvChatAnimation::scrollCallback(lv_timer_t *timer) {
    LvChatAnimation *anim = static_cast<LvChatAnimation *>(timer->user_data);

    // 无限滚动逻辑
    static int scrollY = 0;
    static const int SCROLL_SPEED = 1;

    if (!anim->isCloning) {
        // 创建克隆文本区域
        anim->cloneTextArea = lv_label_create(anim->container);
        lv_obj_add_style(anim->cloneTextArea, &anim->textStyle, 0);
        lv_label_set_long_mode(anim->cloneTextArea, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(anim->cloneTextArea, anim->width - 20);
        lv_label_set_text(anim->cloneTextArea, lv_label_get_text(anim->textArea));
        lv_obj_align(anim->cloneTextArea, LV_ALIGN_TOP_LEFT, 10, anim->height);
        anim->isCloning = true;
        scrollY = 0;
    }

    // 滚动两个文本区域
    scrollY += SCROLL_SPEED;
    lv_obj_set_y(anim->textArea, -scrollY);
    lv_obj_set_y(anim->cloneTextArea, anim->height - scrollY);

    // 检查是否滚动完成
    if (scrollY >= anim->height) {
        // 重置滚动
        lv_obj_del(anim->cloneTextArea);
        anim->cloneTextArea = nullptr;
        lv_obj_set_y(anim->textArea, 10);
        anim->isCloning = false;
        lv_timer_pause(timer);
    }
}

void LvChatAnimation::updateTextDisplay() {
    // 更新当前显示的文本
    std::string displayText = currentMessage.substr(0, charIndex);
    lv_label_set_text(textArea, displayText.c_str());
}

void LvChatAnimation::checkAndScroll() {
    // 检查文本是否超出容器高度
    lv_coord_t textHeight = lv_obj_get_height(textArea);

    if (textHeight > height - 20) {
        // 考虑边距
        // 滚动到底部
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, textArea);
        lv_anim_set_values(&a, lv_obj_get_y(textArea), height - textHeight - 10);
        lv_anim_set_time(&a, 200);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&a, [](void *obj, int32_t value) {
            lv_obj_set_y((lv_obj_t *) obj, value);
        });
        lv_anim_start(&a);
    }
}

void LvChatAnimation::startInfiniteScroll() {
    if (currentState == WAITING) {
        currentState = AUTO_SCROLLING;
        lv_timer_resume(scrollTimer);
    }
}

void LvChatAnimation::addMessage(const char *message) {
    if (!message) return;

    // 停止当前动画
    lv_timer_pause(typeTimer);
    lv_timer_pause(scrollTimer);

    // 清理克隆文本区域
    if (cloneTextArea) {
        lv_obj_del(cloneTextArea);
        cloneTextArea = nullptr;
        isCloning = false;
    }

    // 重置位置
    lv_obj_set_y(textArea, 10);

    // 添加新消息
    messages.push_back(message);
    currentMessage = message;
    charIndex = 0;

    // 开始逐字显示
    currentState = TYPEWRITING;
    lv_label_set_text(textArea, "");
    lv_timer_resume(typeTimer);
}

void LvChatAnimation::setTextStyle(const lv_style_t *style) {
    if (style) {
        //        lv_obj_add_style(textArea, style, 0);
    }
}

void LvChatAnimation::setInfiniteScroll(bool enable) {
    enableInfiniteScroll = enable;
}

void LvChatAnimation::setTypeSpeed(uint32_t speed) {
    typeSpeed = speed;
    if (typeTimer) {
        lv_timer_set_period(typeTimer, speed);
    }
}


// 语音播放回调函数
void playCharacterSound(char c) {
    // 这里实现语音播放逻辑
    // 例如调用音频API播放对应的语音片段
    printf("Playing sound for character: %c\n", c);
}

void lv_example_chat_animation() {
    // 创建聊天动画对象
    static auto chatAnim = new LvChatAnimation(
        lv_scr_act(), // 父对象（屏幕）
        300, // 宽度
        200, // 高度
        findFontBySize(10), // 字体
        10 // 字符显示速度(ms/字符)
    );

    // 设置语音回调
    chatAnim->onCharacterTyped = playCharacterSound;

    // 添加第一条消息
    chatAnim->addMessage(
        "这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.这是第一条消息,会逐字显示出来.");

    // 模拟后续消息（实际应用中可能来自用户输入或网络）
    //    lv_timer_create_once([](lv_timer_t* timer) {
    //        auto* chatAnim = static_cast<LvChatAnimation*>(timer->user_data);
    //        chatAnim->addMessage("这是第二条较长的消息，当文本超出显示区域时，会自动滚动到底部显示最新内容。");
    //    }, 3000, chatAnim);

    //    lv_timer_create_once([](lv_timer_t* timer) {
    //        auto* chatAnim = static_cast<LvChatAnimation*>(timer->user_data);
    //        chatAnim->addMessage("当所有消息都显示完毕且没有新消息时，文本区域会开始无限滚动，展示之前的内容。");
    //    }, 8000, chatAnim);
}
