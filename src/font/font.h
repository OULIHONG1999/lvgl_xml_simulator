//
// Created by Administrator on 2025/4/29.
//

#ifndef FONT_H
#define FONT_H
#include "lvgl/src/font/lv_font.h"
LV_FONT_DECLARE(lv_font_misans_10)
LV_FONT_DECLARE(lv_font_misans_12)
LV_FONT_DECLARE(lv_font_misans_14)

#ifdef __cplusplus
extern "C" {
#endif

// 通过字体名称查找字体，未找到则返回默认字体
extern const lv_font_t *findFontByName(const char *fontName);

// 通过字号查找字体，未找到则返回默认字体
extern const lv_font_t *findFontBySize(int size);

#ifdef __cplusplus
}
#endif

#endif //FONT_H
