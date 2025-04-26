//
// Created by Administrator on 2025/4/26.
//

#ifndef XML_PAGE_CREATE_H
#define XML_PAGE_CREATE_H
#include "../../page_manager/page_manager.h"
#include "src/lib/tinyxml/tinyxml2.h"
#include "src/page/graphic_element/graphic_element.h"

typedef struct {
    GraphicElementType type;
    char *name;
} lv_obj_data_t;

using namespace tinyxml2;

char *copy_string(const char *src);

lv_obj_data_t *set_lv_obj_user_data(GraphicElementType type, const XMLElement *element);

void xml_page_create(const char *xml_path);

GraphicElement *xml_obj_create_element(const XMLElement *element);

void lv_obj_set_base_element(lv_obj_t *obj, GraphicElement *ge);

#endif //XML_PAGE_CREATE_H
