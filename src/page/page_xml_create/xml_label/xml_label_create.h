//
// Created by Administrator on 2025/4/26.
//

#ifndef XML_LABEL_CREATE_H
#define XML_LABEL_CREATE_H
#include "../xml_page_create.h"

lv_obj_t *lv_xml_label_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element);

GraphicElement *xml_label_create_element(const XMLElement *element);
#endif //XML_LABEL_CREATE_H
