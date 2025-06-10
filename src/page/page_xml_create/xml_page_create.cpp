//
// Created by Administrator on 2025/4/26.
//

#include "xml_page_create.h"

#include <iostream>

#include "../../page_manager/page_manager.h"
#include "components/button_factory.h"
#include "components/img_factory.h"
#include "components/label_factory.h"
#include "components/screen_factory.h"
#include "src/lib/tinyxml/tinyxml2.h"
#include "xml_parser/xml_parser.h"


using namespace tinyxml2;


// **解析 XML 生成界面**
void xml_page_create(const char *xml_path) {
    RegisterComponentFactory("screen", new ScreenFactory());
    RegisterComponentFactory("button", new ButtonFactory());
    RegisterComponentFactory("label", new LabelFactory());
    RegisterComponentFactory("img", new ImgFactory());

    XMLDocument doc;
    if (doc.LoadFile(xml_path) != XML_SUCCESS) {
        std::cerr << "XML 文件加载失败！" << std::endl;
        return;
    }

    XMLElement *root = doc.RootElement();
    if (!root) {
        std::cerr << "XML 解析失败，根节点为空！" << std::endl;
        return;
    }

    // **先加载主题**
    XMLElement *themeElem = root->FirstChildElement("theme");
    if (themeElem) {
        StyleManager::LoadTheme(themeElem);
    }

    // **确保 screen 解析正确**
    XMLElement *screenElem = root->FirstChildElement("screen");
    if (!screenElem) {
        std::cerr << "找不到 screen 元素，无法创建界面！" << std::endl;
        return;
    }

    lv_obj_t *screen = lv_scr_act();
    ParseXML(screen, screenElem);
}
