#include "lvgl_main.h"
#include "src/page_manager/page_manager.h"
#include <cstddef>
#include <iostream>
#include "page/page_xml_create/xml_page_create.h"
#include "pika_python/pikaPython.h"
#include "lib/tinyxml/xml_tool.h"

extern void lv_example_scroll_text(void);

extern int test_image_loader();

extern void lv_example_text_animation();

extern void lv_example_chat_animation();

extern void test_animation();

extern void user_test();

extern void lv_example_text_animation();


#ifdef __cplusplus
extern "C" {
#endif

void pikaPython_run() {
    pikaPython pikaPython;
    // pikaScriptShell(pikaScriptInit());
}

void text_animation() {
    lv_example_text_animation();
}



void xml_load(const char *xml_path) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xml_path) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << xml_path << std::endl;
        return;
    }

    tinyxml2::XMLElement *root = doc.RootElement();
    if (!root) {
        std::cerr << "Failed to parse XML, root element is null!" << std::endl;
        return;
    }

    std::cout << "Start traversing XML file: " << xml_path << std::endl;
    traverseXMLNode(&doc,0);
    std::cout << "Finished traversing XML file: " << xml_path << std::endl;
}


void lvgl_main() {
    // xml_load(R"(E:\work_space\LVGL\Simulator\lvgl8_3\lvgl_xml_simulator\src\xml_file\home.xml)");
    xml_page_create(R"(E:\work_space\LVGL\Simulator\lvgl8_3\lvgl_xml_simulator\src\xml_file\home.xml)");
    // xml_page_create(
    //     R"(E:\work_space\LVGL\SimulatorV8.3\lv_simulator_show_xml\src\page\page_xml_create\xml\ui_layout.xml)");

}
#ifdef __cplusplus
}
#endif
