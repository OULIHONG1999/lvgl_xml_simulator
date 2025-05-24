#ifndef LVGL_CHAT_ANIMATION_H
#define LVGL_CHAT_ANIMATION_H

#include "lvgl/lvgl.h"
#include <vector>
#include <string>

class LvChatAnimation {
private:
    // 主容器和参数
    lv_obj_t* container;
    lv_coord_t width, height;
    
    // 文本框参数
    lv_obj_t* textArea;
    lv_style_t textStyle;
    
    // 文本内容管理
    std::vector<std::string> messages;
    std::string currentMessage;
    uint32_t charIndex;
    
    // 动画定时器
    lv_timer_t* typeTimer;
    lv_timer_t* scrollTimer;
    uint32_t typeSpeed; // 字符显示速度(ms/字符)
    
    // 状态控制
    enum State {
        TYPEWRITING,     // 正在逐字显示
        WAITING,         // 等待新消息
        AUTO_SCROLLING   // 自动向上滚动
    };
    State currentState;
    
    // 无限滚动相关
    bool enableInfiniteScroll;
    lv_obj_t* cloneTextArea;
    bool isCloning;
    
    // 回调函数
    static void typeCallback(lv_timer_t* timer);
    static void scrollCallback(lv_timer_t* timer);
    
    // 内部方法
    void updateTextDisplay();
    void checkAndScroll();
    void startInfiniteScroll();
    
public:
    // 构造和析构
    LvChatAnimation(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, 
                    const lv_font_t* font, uint32_t typeSpeed);
    ~LvChatAnimation();
    
    // 禁用拷贝
    LvChatAnimation(const LvChatAnimation&) = delete;
    LvChatAnimation& operator=(const LvChatAnimation&) = delete;
    
    // 添加新消息并开始动画
    void addMessage(const char* message);
    
    // 设置文本样式
    void setTextStyle(const lv_style_t* style);
    
    // 启用/禁用无限滚动
    void setInfiniteScroll(bool enable);
    
    // 控制动画速度
    void setTypeSpeed(uint32_t speed);
    
    // 外部接口：语音播放回调
    void (*onCharacterTyped)(char c); // 每显示一个字符时调用
};

#endif // LVGL_CHAT_ANIMATION_H    