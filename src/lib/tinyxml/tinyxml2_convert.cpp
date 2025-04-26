// tinyxml2_convert.cpp
#include "tinyxml2_convert.h"
#include <stdio.h>

// 创建一个新的 XML 文档
XMLDoc* create_xml_doc() {
    XMLDoc* xmlDoc = new XMLDoc;
    xmlDoc->doc = new tinyxml2::XMLDocument();
    return xmlDoc;
}

// 解析 XML 文件
int parse_xml_file(XMLDoc* xmlDoc, const char* filename) {
    if (xmlDoc && xmlDoc->doc) {
        return xmlDoc->doc->LoadFile(filename);
    }
    return -1;
}

// 获取根元素的名称
const char* get_root_element_name(XMLDoc* xmlDoc) {
    if (xmlDoc && xmlDoc->doc) {
        tinyxml2::XMLElement* root = xmlDoc->doc->RootElement();
        if (root) {
            return root->Name();
        }
    }
    return NULL;
}

// 释放 XML 文档
void free_xml_doc(XMLDoc* xmlDoc) {
    if (xmlDoc) {
        if (xmlDoc->doc) {
            delete xmlDoc->doc;
        }
        delete xmlDoc;
    }
}