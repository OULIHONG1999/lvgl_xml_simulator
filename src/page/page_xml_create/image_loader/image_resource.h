//
// Created by Administrator on 2025/5/15.
//

#ifndef IMAGE_RESOURCE_H
#define IMAGE_RESOURCE_H

#include "image_type.h"
#include <vector>
#include <string>

class ImageResource {
public:
    std::string sourceType;      // 数据来源（文件/网络/SD卡等）
    std::vector<unsigned char> data; // 图片数据
    unsigned int size = 0;       // 文件大小
    unsigned int width = 0;      // 图片宽度
    unsigned int height = 0;     // 图片高度
    ImageType type = ImageType::UNKNOWN; // 图片类型

    void release();
};

#endif // IMAGE_RESOURCE_H
