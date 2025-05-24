#ifndef LVGL_TEXT_ANIMATION_H
#define LVGL_TEXT_ANIMATION_H

#include "lvgl/lvgl.h"
#include <string>
#include <vector>
#include <functional>

// 逐字显示模块
class TextTyper {
private:
    lv_obj_t *label;
    std::string currentText;
    uint32_t charIndex = 0;
    uint32_t typeSpeed = 50; // ms/字符
    bool isTyping = false;
    bool enableFadeIn = false;
    uint32_t fadeDuration = 500; // 淡入持续时间(ms)
    std::vector<uint32_t> charAppearTimes; // 字符出现时间戳
    lv_timer_t *typeTimer = nullptr;
    lv_timer_t *fadeTimer = nullptr;
    std::function<void()> onFinishedCallback;

    // 计算字符透明度(0-255)
    uint8_t calculateOpacity(size_t idx) const;

    // 更新文本显示（带淡入）
    void updateFadeText();

public:
    TextTyper(lv_obj_t *parent, const lv_font_t *font);

    ~TextTyper();

    // 开始逐字显示
    void start(const char *text, uint32_t speedMsPerChar = 50);

    // 暂停/继续
    void pause();

    void resume();

    // 是否完成
    bool isFinished() const { return charIndex >= currentText.length(); }

    // 启用淡入效果
    void enableFadeEffect(bool enable = true, uint32_t durationMs = 500);

    // 设置完成回调
    void setOnFinishedCallback(std::function<void()> cb) { onFinishedCallback = cb; }
    // 获取标签对象
    lv_obj_t *getLabel() const { return label; }
};

// 文本容器（支持动态高度）
class TextContainer {
private:
    lv_obj_t *container;
    lv_coord_t maxHeight = 200; // 最大高度，超出后滚动
    lv_coord_t currentHeight = 100; // 初始高度

public:
    TextContainer(lv_obj_t *parent, lv_coord_t width, lv_coord_t initialHeight = 100);

    // 设置最大高度
    void setMaxHeight(lv_coord_t height) { maxHeight = height; }
    // 获取当前高度
    lv_coord_t getHeight() const { return lv_obj_get_height(container); }

    // 更新内容高度（带动画）
    void updateHeight(lv_coord_t newHeight);

    // 获取LVGL对象
    lv_obj_t *getLvObj() const { return container; }
};

// 滚动管理模块
class ScrollManager {
private:
    lv_obj_t *container;
    lv_obj_t *content; // 文本标签
    lv_coord_t contentHeight = 0;
    bool isAutoScroll = false;
    bool isLoopScroll = false;
    int scrollSpeed = 1; // px/ms
    lv_timer_t *scrollTimer = nullptr;
    lv_obj_t *cloneContent = nullptr; // 双缓冲区克隆对象

    // 自动滚动到底部
    void autoScrollToBottom();

    // 循环滚动逻辑
    static void loopScrollCallback(lv_timer_t *timer);

public:
    ScrollManager(lv_obj_t *container);

    // 关联内容对象
    void attachContent(lv_obj_t *content);

    // 启动自动滚动（内容超出时）
    void startAutoScroll();

    // 启动循环滚动
    void startLoopScroll(int speedPxPerMs = 1);

    // 停止滚动
    void stopScroll();

    // 重置位置
    void resetPosition();
};

#endif // LVGL_TEXT_ANIMATION_H
