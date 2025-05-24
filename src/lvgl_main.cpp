#include "lvgl_main.h"
#include "src/page_manager/page_manager.h"
#include <cstddef>
#include <iostream>
#include "page/page_xml_create/xml_page_create.h"

extern void lv_example_scroll_text(void);

extern int test_image_loader();
extern void lv_example_text_animation();
extern void lv_example_chat_animation();
#ifdef __cplusplus
extern "C" {
#endif


void lvgl_main() {
    // test_image_loader();
    // lv_example_text_animation();
    lv_example_chat_animation();
    // start_activity(&act_home, NULL);
    // start_activity(&act_img_test, NULL);

    // xml_page_create(
    //     R"(E:\work_space\LVGL\SimulatorV8.3\lv_simulator_show_xml\src\page\page_xml_create\xml\ui_layout.xml)");

    // lv_example_scroll_text();
}
#ifdef __cplusplus
}
#endif
