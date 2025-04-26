//
// Created by Administrator on 2025/4/26.
//

#ifndef XML_IMG_CREATE_H
#define XML_IMG_CREATE_H
#include "../xml_page_create.h"

lv_obj_t *lv_xml_img_create(lv_obj_t *parent, GraphicElement *ge, const XMLElement *element);
GraphicElement *xml_img_create_element(const XMLElement *element);
#endif //XML_IMG_CREATE_H
