// tinyxml2_convert.h
#ifndef TINYXML2_CONVERT_H
#define TINYXML2_CONVERT_H

#ifdef __cplusplus
// C++ 环境下的包含
#include "tinyxml2.h"
#else
// C 环境下的包含
// 这里需要确认 tinyxml2.h 能否在 C 环境下使用
// 并且手动处理可能需要的 C 标准库头文件
#include <ctype.h> // 处理 cctype 对应的 C 头文件
#include "tinyxml2.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        tinyxml2::XMLDocument* doc;
    } XMLDoc;

    XMLDoc* create_xml_doc();
    int parse_xml_file(XMLDoc* xmlDoc, const char* filename);
    const char* get_root_element_name(XMLDoc* xmlDoc);
    void free_xml_doc(XMLDoc* xmlDoc);

#ifdef __cplusplus
}
#endif

#endif //TINYXML2_CONVERT_H