//
// Created by Administrator on 2025/6/10.
//

#ifndef SCREEN_FACTORY_H
#define SCREEN_FACTORY_H

#include "base_factory.h"
#include "src/page/page_xml_create/style/style_manager.h"

// **屏幕工厂**
class ScreenFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *screen = lv_obj_create(parent);
        StyleManager::ApplyAttributes(screen, xmlElem);
        return screen;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};


#endif //SCREEN_FACTORY_H
