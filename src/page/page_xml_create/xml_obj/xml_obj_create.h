//
// Created by Administrator on 2025/4/26.
//

#ifndef XML_OBJ_CREATE_H
#define XML_OBJ_CREATE_H
#include "../xml_page_create.h"

lv_obj_t *lv_xml_obj_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element);

// 处理Element
GraphicElement *xml_obj_create_element(const XMLElement *element);

#endif //XML_OBJ_CREATE_H
