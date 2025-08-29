//
// Created by Administrator on 2025/8/29.
//

#ifndef VIEW_FACTORY_H
#define VIEW_FACTORY_H

#include "base_factory.h"
#include "src/page/page_xml_create/style/style_manager.h"

// **View容器工厂 - 用于处理布局容器**
class ViewFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        // 创建一个基础对象作为容器
        lv_obj_t *container = lv_obj_create(parent);

        // 移除默认边框和背景以使其更像一个纯粹的容器
        lv_obj_remove_style_all(container);

        //设置对象属性
        SetObjAttributes(container, xmlElem);

        return container;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        if (!obj || !xmlElem) return;
        SetCommonAttributes(obj, xmlElem);
        // 强制刷新对象
        lv_obj_invalidate(obj);

        // 控件特殊属性处理
        // 取得对象名称
        const char *name = xmlElem->Attribute("name");
        // 打印一下长宽
        printf("View name: %s\n", name);
        printf("View width: %d\n", lv_obj_get_width(obj));
        printf("View height: %d\n", lv_obj_get_height(obj));
    }
};

#endif //VIEW_FACTORY_H
