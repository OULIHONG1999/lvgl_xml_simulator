//
// Created by Administrator on 2025/6/10.
//

#ifndef BASE_FACTORY_H
#define BASE_FACTORY_H

#include <unordered_map>
#include <string>

#include "../../page_manager/page_manager.h"
#include "src/lib/tinyxml/tinyxml2.h"

#include "../../pika_python/pikaPython.h"

using namespace tinyxml2;


class LVGLComponentFactory {
public:
    virtual ~LVGLComponentFactory() = default;

    virtual lv_obj_t *CreateComponent(lv_obj_t *parent, tinyxml2::XMLElement *xmlElem) = 0;

    virtual void SetObjAttributes(lv_obj_t *obj, tinyxml2::XMLElement *xmlElem) = 0;
};


#endif //BASE_FACTORY_H
