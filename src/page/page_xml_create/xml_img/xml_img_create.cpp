//
// Created by Administrator on 2025/4/26.
//

#include "xml_img_create.h"
lv_obj_t *lv_xml_img_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element) {
    lv_obj_t *obj = lv_img_create(parent);
    lv_obj_set_base_element(obj, ge);
    // lv_img_set_src(obj, src);
    lv_obj_set_user_data(obj, set_lv_obj_user_data(GRAPHIC_BASE, element));
    return obj;
}
GraphicElement *xml_img_create_element(const XMLElement *element) {
    GraphicElement *graphicElement = xml_obj_create_element(element);
    graphicElement->type = GRAPHIC_IMG;
    // 读取img的src属性
    const char *src = element->Attribute("src");
    if (src != nullptr) {
        // 复制src到graphicElement->img.src
        graphicElement->img.src = copy_string(src);
    } else {
        graphicElement->img.src = copy_string("lose src");
    }
    printf("img_src: %s\n", graphicElement->img.src);
    return graphicElement;
}