//
// Created by Administrator on 2025/6/10.
//

#ifndef LABEL_FACTORY_H
#define LABEL_FACTORY_H
#include "base_factory.h"
#include "src/page/page_xml_create/style/style_manager.h"


// **标签工厂**
class LabelFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *label = lv_label_create(parent);

        SetObjAttributes(label, xmlElem);

        return label;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        if (!obj || !xmlElem) return;

        // 设置通用属性（尺寸、对齐、样式等）
        SetCommonAttributes(obj, xmlElem);

        // 设置默认字体（可根据需要改为动态字体）
        lv_obj_set_style_text_font(obj, findFontBySize(10), LV_PART_MAIN);

        // 设置文本内容
        if (const char *text = xmlElem->Attribute("text")) {
            lv_label_set_text(obj, text);
        }

        // 设置文本对齐方式
        if (const char *align = xmlElem->Attribute("text_align")) {
            if (strcmp(align, "center") == 0) {
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
            } else if (strcmp(align, "right") == 0) {
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
            } else {
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
            }
        }

        // 数据绑定：bind_text + bind_text-fmt
        const char* bindText = xmlElem->Attribute("bind_text");
        const char* bindFmt  = xmlElem->Attribute("bind_text-fmt");
        // if (bindText && std::strlen(bindText) > 0) {
        //     lv_subject_t* subject = lv_subject_get_by_name(bindText); // 伪代码，需你实现
        //     if (subject) {
        //         lv_label_bind_text(obj, subject, bindFmt ? bindFmt : "%s"); // 支持格式化
        //     }
        // }
    }
};

#endif //LABEL_FACTORY_H
