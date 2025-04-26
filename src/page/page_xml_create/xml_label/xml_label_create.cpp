//
// Created by Administrator on 2025/4/26.
//

#include "xml_label_create.h"
lv_obj_t *lv_xml_label_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element) {
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_base_element(label, ge);
    // 读取label的text属性
    const char *text = element->Attribute("text");
    if (text == nullptr)
        lv_label_set_text(label, "lose text");
    else
        lv_label_set_text(label, text);
    lv_obj_set_user_data(label, set_lv_obj_user_data(GRAPHIC_BASE, element));
    return label;
}
GraphicElement *xml_label_create_element(const XMLElement *element) {
    GraphicElement *graphicElement = xml_obj_create_element(element);
    graphicElement->type = GRAPHIC_LABEL;
    // 读取label的text属性
    const char *text = element->Attribute("text");
    if (text != nullptr) {
        // 复制text到graphicElement->label.text
        graphicElement->label.text = copy_string(text);
    } else {
        // 尝试去子元素获取text, 如果实在找不到就设置为"lose text"
        const XMLElement *e = element->FirstChildElement("text");
        if (e != nullptr) {
            // 复制text到graphicElement->label.text
            graphicElement->label.text = copy_string(e->GetText());
        } else {
            graphicElement->label.text = copy_string("lose text");
        }
    }
    printf("label_text: %s\n", graphicElement->label.text);
    return graphicElement;
}