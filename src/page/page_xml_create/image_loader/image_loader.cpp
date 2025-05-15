//
// Created by Administrator on 2025/5/15.
//

#include "image_loader.h"

#include <cstring>
#include <iostream>




// 默认实现：通用图片加载失败处理
bool ImageLoader::LoadImage(const std::string &filePath, ImageResource &resource) {
    std::cerr << "Error: ImageLoader does not support direct loading. Use a specialized loader." << std::endl;
    return false;
}


