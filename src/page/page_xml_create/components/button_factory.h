//
// Created by Administrator on 2025/6/10.
//

#ifndef BUTTON_FACTORY_H
#define BUTTON_FACTORY_H

#include "base_factory.h"
#include "src/page/page_xml_create/style/style_manager.h"

// **按钮工厂**
class ButtonFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *btn = lv_btn_create(parent);

        // 先设置基本属性（包括样式）
        SetObjAttributes(btn, xmlElem);

        // 查找 <script> 标签
        if (XMLElement *scriptElem = xmlElem->FirstChildElement("script")) {
            if (const char *scriptText = scriptElem->GetText()) {
                printf("script: %s\n", scriptText);
                pikaPython().run((char *) scriptText);
            }
        }
        return btn;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        // 解析 part（默认 LV_PART_MAIN）
        lv_part_t part = LV_PART_MAIN;
        if (const char *partAttr = xmlElem->Attribute("part")) {
            std::string partStr = partAttr;
            if (partStr == "main") part = LV_PART_MAIN;
            else if (partStr == "scrollbar") part = LV_PART_SCROLLBAR;
            else if (partStr == "indicator") part = LV_PART_INDICATOR;
            else if (partStr == "knob") part = LV_PART_KNOB;
            else if (partStr == "selected") part = LV_PART_SELECTED;
            // TODO: 继续补齐需要的 LVGL 部位映射
        }

        // 应用 style（如果有）
        if (const char *styleName = xmlElem->Attribute("style")) {
            if (!StyleManager::ApplyStyleToObj(styleName, obj, part)) {
                printf("⚠️ can't find style! : %s\n", styleName);
            }
        }

        // 处理 label 属性
        if (const char *labelText = xmlElem->Attribute("label");
            labelText && std::strlen(labelText) > 0) {
            lv_obj_t *label = lv_label_create(obj);
            lv_label_set_text(label, labelText);
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(label, findFontBySize(10), 0);
        }
    }
};

// 静态变量定义
std::unordered_map<std::string, lv_style_t*> StyleManager::namedStyles;

#endif //BUTTON_FACTORY_H
