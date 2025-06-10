//
// Created by Administrator on 2025/6/10.
//

#include "xml_parser.h"

// **组件映射表**
std::unordered_map<std::string, LVGLComponentFactory *> componentFactories;

// **递归解析 XML 并创建 LVGL 组件**
void ParseXML(lv_obj_t *parent, XMLElement *xmlElem) {
    if (!xmlElem) return;

    std::string tagName = xmlElem->Name();
    printf("Parsing tag: %s\n", tagName.c_str());

    auto factoryIt = componentFactories.find(tagName);
    if (factoryIt != componentFactories.end()) {
        lv_obj_t *obj = factoryIt->second->CreateComponent(parent, xmlElem);
        // 递归解析子组件
        XMLElement *child = xmlElem->FirstChildElement();
        if (!child) {
            printf("End of branch: %s has no children.\n", tagName.c_str());
        } else {
            while (child) {
                printf("Child tag: %s\n", child->Name());
                ParseXML(obj, child);
                child = child->NextSiblingElement();
            }
        }
    } else {
        printf("No factory found for tag: %s\n", tagName.c_str());
    }
}



void RegisterComponentFactory(const std::string &tagName, LVGLComponentFactory *factory) {
    componentFactories[tagName] = factory;
}