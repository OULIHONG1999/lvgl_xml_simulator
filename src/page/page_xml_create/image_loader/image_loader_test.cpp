//
// Created by Administrator on 2025/5/15.
//

#include <iostream>
#include <optional>
#include <lvgl/lvgl.h>
#include "image_factory.h"


int test_image_loader() {
    std::string imagePath =
            R"(E:\work_space\LVGL\SimulatorV8.3\lv_simulator_show_xml\src\res\img\icon.png)";
    static ImageResource res;
    auto image = loader_image(imagePath, res);
    if (image) {
        // 处理加载的图像数据
        std::cout << "Image data: " << image->data.size() << " bytes" << std::endl;
    }else {
        std::cerr << "Failed to load image." << std::endl;
    }

    // 创建 LVGL 图像描述符
    static lv_img_dsc_t img_dsc;
    img_dsc.header.always_zero = 0;
    img_dsc.header.w = res.width;
    img_dsc.header.h = res.height;
    img_dsc.data = res.data.data();  // 确保数据格式匹配
    img_dsc.data_size = res.size;  // 假设 size 是字节数
    img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;  // 根据实际颜色格式设置，如 LV_IMG_CF_TRUE_COLOR_ALPHA

    // 创建图像对象
    lv_obj_t* img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &img_dsc);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);


    return 0;
}
