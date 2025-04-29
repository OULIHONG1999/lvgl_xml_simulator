//
// Created by Administrator on 2025/4/29.
//

#ifndef EXTRACT_PADDING_H
#define EXTRACT_PADDING_H
#include "lvgl/src/misc/lv_area.h"

// 定义默认颜色
#define DEFAULT_COLOR 0x000000

// 定义存储 padding 值的结构体
typedef struct {
    lv_coord_t top;
    lv_coord_t right;
    lv_coord_t bottom;
    lv_coord_t left;
} Padding;


int extract_value(const char *str, int *index);

// 从 padding 字符串中提取四个值到结构体
void extract_padding_values(const char *padding, Padding *p);

// 打印提取的 padding 值
void print_padding_values(const Padding *p);

// 将颜色字符串转换为十六进制值
unsigned int color_string_to_hex(const char *color_str);

lv_align_t get_lv_align(const char *str);

lv_coord_t get_lv_px_value(const char *str);

#endif //EXTRACT_PADDING_H
