//
// Created by Administrator on 2025/6/10.
//

#ifndef STYLE_MANAGER_H
#define STYLE_MANAGER_H

#include "../components/base_factory.h"
#include <vector>

class StyleManager {
public:

    // 存储LVGL样式对象的映射表
    // 键是样式名称，值是lv_style_t指针
    static std::unordered_map<std::string, lv_style_t*> namedStyles;

    static void ParseTheme(XMLElement *themeElem) {
        if (!themeElem) return;

        // 获取样式名称
        const char *themeIdAttr = themeElem->Attribute("name");
        if (!themeIdAttr) return;
        std::string themeName = themeIdAttr;

        // 每次都新建一个 lv_style_t
        auto *style = new lv_style_t;   // ✅ 在堆上分配
        lv_style_init(style);                 // 初始化样式结构体

        // 解析 <style> 子标签并设置属性
        for (XMLElement *styleElem = themeElem->FirstChildElement("style");
             styleElem != nullptr;
             styleElem = styleElem->NextSiblingElement("style")) {
            ParseStyle(styleElem, style);      // 你的属性解析函数
             }
        // 存入映射表
        namedStyles[themeName] = style;
    }


    static void ParseStyle(XMLElement *styleElem, lv_style_t *style) {
        if (!styleElem || !style) return;

        // 遍历所有属性
        const XMLAttribute *attr = styleElem->FirstAttribute();
        while (attr) {
            std::string attrName = attr->Name();
            std::string attrValue = attr->Value();

            // 常见属性映射
            if (attrName == "bg_color") {
                lv_color_t c = ParseColor(attrValue);
                lv_style_set_bg_color(style, c);
            }
            else if (attrName == "bg_opa") {
                lv_style_set_bg_opa(style, static_cast<lv_opa_t>(std::stoi(attrValue)));
            }
            else if (attrName == "radius") {
                lv_style_set_radius(style, std::stoi(attrValue));
            }
            else if (attrName == "border_color") {
                lv_color_t c = ParseColor(attrValue);
                lv_style_set_border_color(style, c);
            }
            else if (attrName == "border_width") {
                lv_style_set_border_width(style, std::stoi(attrValue));
            }
            else if (attrName == "text_color") {
                lv_color_t c = ParseColor(attrValue);
                lv_style_set_text_color(style, c);
            }
            // TODO: 继续扩展其他 LVGL 支持的样式属性

            attr = attr->Next();
        }
    }

    static bool ApplyStyleToObj(const std::string &styleName, lv_obj_t *obj, lv_part_t part = LV_PART_MAIN) {
        if (!obj) return false;

        auto it = namedStyles.find(styleName);
        if (it != namedStyles.end() && it->second) {
            lv_obj_add_style(obj, it->second, part);
            return true; // 应用成功
        }

        // 可选：调试输出
        printf("⚠️ style '%s' unfind\n", styleName.c_str());
        return false;
    }


private:
    static lv_color_t ParseColor(const std::string &hexStr) {
        // 支持 "#RRGGBB" 或 "0xRRGGBB"
        unsigned int rgb = 0;
        if (hexStr.rfind("#", 0) == 0) {
            rgb = std::stoul(hexStr.substr(1), nullptr, 16);
        } else if (hexStr.rfind("0x", 0) == 0 || hexStr.rfind("0X", 0) == 0) {
            rgb = std::stoul(hexStr.substr(2), nullptr, 16);
        } else {
            rgb = std::stoul(hexStr, nullptr, 16);
        }
        return lv_color_make((rgb >> 16) & 0xFF,
                             (rgb >> 8) & 0xFF,
                             rgb & 0xFF);
    }
};


#endif //STYLE_MANAGER_H
