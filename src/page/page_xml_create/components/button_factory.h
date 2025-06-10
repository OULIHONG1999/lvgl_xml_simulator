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
        StyleManager::ApplyAttributes(btn, xmlElem);
        SetObjAttributes(btn, xmlElem);

        // 查找其是否有子元素为script标签的元素
        XMLElement *scriptElem = xmlElem->FirstChildElement("script");
        if (scriptElem) {
            std::string script = scriptElem->GetText();
            printf("script: %s\n", script.c_str());

            pikaPython().run((char *) script.c_str());
        }


        return btn;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        // 查找是否有文本标签
        if (const char *text = xmlElem->Attribute("text"); text && std::strlen(text) > 0) {
            lv_obj_t *label = lv_label_create(obj);
            lv_label_set_text(label, text);
            // 居中
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            // 设置默认字体
            lv_obj_set_style_text_font(label, findFontBySize(10), 0);
        }
    }
};


#endif //BUTTON_FACTORY_H
