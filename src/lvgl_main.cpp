#include "lvgl_main.h"
#include "src/page_manager/page_manager.h"
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include "lib/tinyxml/tinyxml2_convert.h"
#include "page/page_xml_create/xml_page_create.h"

extern int GraphicElement_test_main();


#ifdef __cplusplus
extern "C" {
#endif


void lvgl_main() {
    // start_activity(&act_home, NULL);
    xml_page_create(
        R"(E:\work_space\LVGL\SimulatorV8.3\lv_simulator_show_xml\src\page\page_home\xml\activity_home2.xml)");
    // GraphicElement_test_main();
}
#ifdef __cplusplus
}
#endif
