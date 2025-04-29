//
// Created by Administrator on 2025/4/26.
//

#include "xml_button_create.h"


using namespace tinyxml2;

lv_obj_t *lv_xml_button_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element) {
    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_base_element(button, ge);

    // 设置pad
    lv_obj_set_style_pad_all(button, 0, 0);

    lv_obj_t *label = lv_label_create(button);
    lv_obj_center(label);
    const char *text = element->Attribute("text");
    if (text == nullptr)
        lv_label_set_text(label, "lose text");
    else
        lv_label_set_text(label, text);

    lv_obj_set_user_data(button, set_lv_obj_user_data(GRAPHIC_BASE, element));
    return button;
}

GraphicElement *xml_button_create_element(const XMLElement *element) {
    GraphicElement *graphicElement = xml_obj_create_element(element);
    graphicElement->type = GRAPHIC_BUTTON;
    // 读取button的text属性
    const char *text = element->Attribute("text");
    if (text != nullptr) {
        // 复制text到graphicElement->button.text
        graphicElement->button.label.text = copy_string(text);
    } else {
        // 尝试去子元素获取text, 如果实在找不到就设置为"lose text"
        const XMLElement *e = element->FirstChildElement("text");
        if (e != nullptr) {
            // 复制text到graphicElement->button.text
            graphicElement->button.label.text = copy_string(e->GetText());
        } else {
            graphicElement->button.label.text = copy_string("lose text");
        }
    }
    printf("button_text: %s\n", graphicElement->button.label.text);
    return graphicElement;
}