//
// Created by Administrator on 2025/6/10.
//

#include "xml_parser.h"

#include "src/page/page_xml_create/style/style_manager.h"

// **组件映射表 - 存储标签名到工厂对象的映射关系**
// 这个哈希表用于存储所有已注册的组件工厂，键是XML标签名，值是对应的工厂对象指针
std::unordered_map<std::string, LVGLComponentFactory *> componentFactories;

/**
 * 递归解析 XML 并创建 LVGL 组件
 * @param parent 父级LVGL对象，新创建的组件将作为其子对象
 * @param xmlElem 当前要解析的XML元素
 *
 * 该函数会根据XML元素的标签名查找对应的组件工厂，
 * 然后使用工厂创建相应的LVGL对象，并递归处理其子元素
 * 支持的标签包括: screen, button, label, img, view 等
 */
void ParseXML(lv_obj_t *parent, XMLElement *xmlElem) {
    // 检查输入参数是否有效
    if (!xmlElem) return;

    // 获取当前XML元素的标签名
    std::string tagName = xmlElem->Name();
    printf("Parsing tag: %s\n", tagName.c_str());
    // 特殊处理style标签
    if (tagName == "style") {
        // style 标签不创建对象，而是加载主题样式
        StyleManager::ParseTheme(xmlElem);
        return;
    }

    // 在组件工厂映射表中查找对应标签名的工厂
    auto factoryIt = componentFactories.find(tagName);
    if (factoryIt != componentFactories.end()) {
        // 找到对应的工厂，使用工厂创建组件
        lv_obj_t *obj = factoryIt->second->CreateComponent(parent, xmlElem);

        // 递归解析子组件
        XMLElement *child = xmlElem->FirstChildElement();
        if (!child) {
            // 当前元素没有子元素
            printf("End of branch: %s has no children.\n", tagName.c_str());
        } else {
            // 遍历并处理所有子元素
            while (child) {
                printf("Child tag: %s\n", child->Name());
                ParseXML(obj, child);  // 递归调用处理子元素
                child = child->NextSiblingElement();
            }
        }
    } else {
        // 未找到对应标签的工厂
        printf("No factory found for tag: %s\n", tagName.c_str());
    }
}

/**
 * 注册组件工厂
 * @param tagName XML标签名
 * @param factory 对应的组件工厂对象指针
 *
 * 用法示例:
 * RegisterComponentFactory("button", new ButtonFactory());
 * RegisterComponentFactory("label", new LabelFactory());
 * RegisterComponentFactory("screen", new ScreenFactory());
 *
 * 该函数用于将特定标签名与组件工厂关联起来，
 * 使得ParseXML函数能够根据标签名找到对应的工厂来创建组件
 */
void RegisterComponentFactory(const std::string &tagName, LVGLComponentFactory *factory) {
    componentFactories[tagName] = factory;
}
