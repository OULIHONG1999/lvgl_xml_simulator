//
// Created by Administrator on 2025/4/26.
//

#include "xml_obj_create.h"
lv_obj_t *lv_xml_obj_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element) {
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_base_element(obj, ge);
    lv_obj_set_user_data(obj, set_lv_obj_user_data(GRAPHIC_BASE, element));
    return obj;
}

// 处理Element
GraphicElement *xml_obj_create_element(const XMLElement *element) {
    GraphicElement *graphicElement = create_default_graphic_element();
    if (graphicElement == NULL) {
        printf("create_default_graphic_element error\n");
        return NULL;
    }
    element->QueryIntAttribute("id", &graphicElement->id);
    element->QueryIntAttribute("x", &graphicElement->x);
    element->QueryIntAttribute("y", &graphicElement->y);
    element->QueryIntAttribute("w", &graphicElement->w);
    element->QueryIntAttribute("h", &graphicElement->h);
    print_graphic_element(graphicElement);
    return graphicElement;
}