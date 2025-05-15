//
// Created by Administrator on 2025/5/15.
//

#include "png_image_loader.h"

#include <cstring>
#include <fstream>
#include <iostream>

bool PNGImageLoader::LoadImage(const std::string& filePath, ImageResource& resource) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open PNG file: " << filePath << std::endl;
        return false;
    }

    resource.size = file.tellg();
    file.seekg(0, std::ios::beg);
    resource.data.resize(resource.size);

    if (!file.read(reinterpret_cast<char*>(resource.data.data()), resource.size)) {
        std::cerr << "Error: Failed to read PNG data: " << filePath << std::endl;
        return false;
    }

    resource.sourceType = "File";
    resource.type = ImageType::PNG;

    // 直接解析 PNG 头部信息
    if (resource.size >= 24) {
        const unsigned char *data = resource.data.data();

        // 检查 PNG 文件签名
        if (memcmp(data, "\x89PNG\r\n\x1a\n", 8) != 0) {
            std::cerr << "Invalid PNG signature: " << filePath << std::endl;
            return false;
        }

        // 确定 IHDR 头部位置
        const unsigned char *ihdr = data + 8 + 4;
        if (memcmp(ihdr, "IHDR", 4) != 0) {
            std::cerr << "Invalid IHDR chunk: " << filePath << std::endl;
            return false;
        }

        // 解析宽度和高度（大端格式）
        resource.width = (ihdr[4] << 24) | (ihdr[5] << 16) | (ihdr[6] << 8) | ihdr[7];
        resource.height = (ihdr[8] << 24) | (ihdr[9] << 16) | (ihdr[10] << 8) | ihdr[11];

    } else {
        std::cerr << "Invalid PNG file: " << filePath << std::endl;
        return false;
    }

    return true;
}
