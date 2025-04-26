#ifndef GRAPHIC_ELEMENT_H
#define GRAPHIC_ELEMENT_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    GRAPHIC_IMG,
    GRAPHIC_LABEL,
    GRAPHIC_BUTTON
} GraphicElementType;

typedef struct {
    // lvgl 通用属性
    int outline;
    unsigned int background_color;
    unsigned int text_color;
    int font_size;
    int padding_left;
    int padding_right;
    int padding_top;
    int padding_bottom;
    int border_width;
    unsigned int border_color;
    int radius;
    int shadow_width;
    unsigned int shadow_color;
    int shadow_offset_x;
    int shadow_offset_y;
    int opacity;
    int visible;
} GraphicBase;

typedef struct {
    char *src;
} GraphicImg;

typedef struct {
    char *text;
} GraphicLabel;

typedef struct {
    GraphicLabel label;
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
