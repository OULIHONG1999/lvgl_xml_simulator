#ifndef GRAPHIC_ELEMENT_H
#define GRAPHIC_ELEMENT_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    GRAPHIC_BASE,
    GRAPHIC_IMG,
    GRAPHIC_LABEL,
    GRAPHIC_BUTTON
} GraphicElementType;

typedef enum {
    IMG_GIF,
    IMG_JPEG,
    IMG_PNG,
    IMG_BMP,
} ImgType;

/**
 * @brief 定义图形元素的通用属性结构体，包含 LVGL 相关的通用样式属性。
 *
 * 该结构体存储了图形元素的各种样式属性，如轮廓、背景颜色、文本颜色等，
 * 可用于统一管理和设置不同图形元素的样式。
 */
typedef struct {
    int outline;                // 轮廓相关设置，具体含义取决于 LVGL 框架的实现
    unsigned int background_color; // 背景颜色，使用无符号整数表示，具体编码格式由 LVGL 决定
    unsigned int text_color;    // 文本颜色，使用无符号整数表示，具体编码格式由 LVGL 决定
    int font_size;              // 字体大小，单位可能由 LVGL 框架定义
    int padding_left;           // 左内边距，即元素内容与左侧边框的距离
    int padding_right;          // 右内边距，即元素内容与右侧边框的距离
    int padding_top;            // 上内边距，即元素内容与上侧边框的距离
    int padding_bottom;         // 下内边距，即元素内容与下侧边框的距离
    int border_width;           // 边框宽度，用于设置元素边框的粗细
    unsigned int border_color;  // 边框颜色，使用无符号整数表示，具体编码格式由 LVGL 决定
    int radius;                 // 圆角半径，用于设置元素边框的圆角程度
    int shadow_width;           // 阴影宽度，用于设置元素阴影的宽度
    unsigned int shadow_color;  // 阴影颜色，使用无符号整数表示，具体编码格式由 LVGL 决定
    int shadow_offset_x;        // 阴影在 x 轴方向的偏移量
    int shadow_offset_y;        // 阴影在 y 轴方向的偏移量
    int opacity;                // 元素的不透明度，取值范围可能由 LVGL 框架定义
    int visible;                // 元素的可见性，具体值的含义由 LVGL 框架决定
} GraphicBase;



/**
 * @brief 定义图形图像元素结构体，存储图像源信息。
 */
typedef struct {
    ImgType type;               // 图像源类型，如文件路径或资源标识符
    char *src;                  // 图像源文件路径或标识符
    int img_width;              // 图像的宽度
    int img_height;             // 图像的高度
    int img_rotation;           // 图像的旋转角度，单位为度
} GraphicImg;

/**
 * @brief 定义图形标签元素结构体，存储标签文本信息。
 */
typedef struct {
    char *text;                 // 标签显示的文本内容
    int text_align;             // 文本对齐方式，具体值含义由 LVGL 框架定义
    int line_space;             // 文本行间距
} GraphicLabel;

/**
 * @brief 定义图形按钮元素结构体，包含按钮上的标签信息。
 */
typedef struct {
    GraphicLabel label;         // 按钮上显示的标签
    int button_state;           // 按钮的状态，如按下、释放等，具体值含义由 LVGL 框架定义
    int clickable;              // 按钮是否可点击，1 表示可点击，0 表示不可点击
} GraphicButton;

// 定义图形界面元素结构体
typedef struct GraphicElement {
    GraphicElementType type;
    int id;
    int x;
    int y;
    int w;
    int h;
    GraphicBase base;
    union {
        GraphicImg img;
        GraphicLabel label;
        GraphicButton button;
    };
    void *lv_obj;
    void (*free_lv_obj)(void *obj);
} GraphicElement;

// 创建图形元素
GraphicElement *create_graphic_element(int x, int y, int width, int height);

// 创建默认图形元素
GraphicElement *create_default_graphic_element();

// 释放图形元素
void free_graphic_element(void *element);

// 打印图形元素信息
void print_graphic_element(GraphicElement *element);

#endif
