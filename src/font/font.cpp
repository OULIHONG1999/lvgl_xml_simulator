//
// Created by Administrator on 2025/4/29.
//
#include "font.h"
#include <map>
#include <string>
// 实现字体映射表
const std::map<std::string, const lv_font_t *> font_map = {
    {"lv_font_misans_10", &lv_font_misans_10},
    {"lv_font_misans_12", &lv_font_misans_12},
    {"lv_font_misans_14", &lv_font_misans_14}

};


// 定义默认字体，这里假设默认字体为 lv_font_misans_12
const lv_font_t *default_font = &lv_font_misans_12;

// 通过字体名称查找字体，未找到则返回默认字体
const lv_font_t *findFontByName(const  char *fontName) {
    auto it = font_map.find(fontName);
    if (it != font_map.end()) {
        return it->second;
    }
    return default_font;
}

// 通过字号查找字体，未找到则返回默认字体
const lv_font_t *findFontBySize(int size) {
    std::string fontName = "lv_font_misans_" + std::to_string(size);
    return findFontByName(fontName.c_str());
}
