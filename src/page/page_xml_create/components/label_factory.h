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
        StyleManager::ApplyAttributes(label, xmlElem);
        SetObjAttributes(label, xmlElem);
        return label;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        // 设置默认字体
        lv_obj_set_style_text_font(obj, findFontBySize(10), 0);
        // 设置标签的文本
        if (const char *text = xmlElem->Attribute("text"); text && std::strlen(text) > 0) {
            lv_label_set_text(obj, text);
        } else
            lv_label_set_text(obj, "");
    }
};

#endif //LABEL_FACTORY_H
