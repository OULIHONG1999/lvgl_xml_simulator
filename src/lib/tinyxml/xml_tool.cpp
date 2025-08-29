//
// Created by Administrator on 2025/8/29.
//

#include "xml_tool.h"

#include <iostream>

#include "tinyxml2.h"


// 递归遍历XML节点并打印信息
void traverseXMLNode(tinyxml2::XMLNode *node, int depth) {
    if (node == nullptr) return;

    // 打印缩进
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }

    // 打印节点信息
    if (node->ToElement()) {
        tinyxml2::XMLElement *element = node->ToElement();
        std::cout << "<" << element->Name();

        // 打印属性
        for (const tinyxml2::XMLAttribute *attr = element->FirstAttribute(); attr != nullptr; attr = attr->Next()) {
            std::cout << " " << attr->Name() << "=\"" << attr->Value() << "\"";
        }
        std::cout << ">" << std::endl;
    } else if (node->ToText()) {
        tinyxml2::XMLText *text = node->ToText();
        std::cout << "Text: " << text->Value() << std::endl;
    } else if (node->ToComment()) {
        tinyxml2::XMLComment *comment = node->ToComment();
        std::cout << "Comment: " << comment->Value() << std::endl;
    } else {
        std::cout << "Unknown node type" << std::endl;
    }

    // 递归遍历子节点
    for (tinyxml2::XMLNode *child = node->FirstChild(); child != nullptr; child = child->NextSibling()) {
        traverseXMLNode(child, depth + 1);
    }

    // 打印结束标签
    if (node->ToElement()) {
        tinyxml2::XMLElement *element = node->ToElement();
        for (int i = 0; i < depth; i++) {
            std::cout << "  ";
        }
        std::cout << "</" << element->Name() << ">" << std::endl;
    }
}