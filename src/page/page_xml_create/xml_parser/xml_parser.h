//
// Created by Administrator on 2025/6/10.
//

#ifndef XML_PARSER_H
#define XML_PARSER_H


#include "../components/base_factory.h"

extern std::unordered_map<std::string, LVGLComponentFactory*> componentFactories;

void RegisterComponentFactory(const std::string& tagName, LVGLComponentFactory* factory);
void ParseXML(lv_obj_t* parent, tinyxml2::XMLElement* xmlElem);

#endif //XML_PARSER_H
