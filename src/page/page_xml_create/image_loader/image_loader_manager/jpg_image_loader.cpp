//
// Created by Administrator on 2025/5/15.
//

#include "jpg_image_loader.h"

#include <cstring>
#include <fstream>
#include <iostream>

bool JPGImageLoader::LoadImage(const std::string &filePath, ImageResource &resource) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open JPG file: " << filePath << std::endl;
        return false;
    }

    resource.size = file.tellg();
    file.seekg(0, std::ios::beg);
    resource.data.resize(resource.size);

    if (!file.read(reinterpret_cast<char *>(resource.data.data()), resource.size)) {
        std::cerr << "Error: Failed to read JPG data: " << filePath << std::endl;
        return false;
    }

    resource.sourceType = "File";
    resource.type = ImageType::JPG;

    // 解析 JPEG 头部信息
    const unsigned char *data = resource.data.data();
    if (resource.size < 3 || memcmp(data, "\xff\xd8\xff", 3) != 0) {
        std::cerr << "Invalid JPEG signature: " << filePath << std::endl;
        // 输出读到的数据
        for (int i = 0; i < 10; i++) {
            std::cout << "0X" << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        return false;
    }

    const unsigned char *ptr = data + 2;
    while (ptr < data + resource.size) {
        if (*ptr++ != 0xff) continue; // 找到标记起始位置
        uint8_t marker = *ptr++;

        // 打印标记（可选调试）
        // std::cout << "Marker: 0x" << std::hex << static_cast<int>(marker) << std::dec << std::endl;

        // 检查 SOF（Start of Frame）标记，获取宽高
        if ((marker & 0xf0) == 0xc0 && marker != 0xc4 && marker != 0xc8) {
            resource.height = (ptr[3] << 8) | ptr[4];
            resource.width = (ptr[5] << 8) | ptr[6];

            return true;
        }

        // 跳过当前块
        ptr += (ptr[0] << 8) | ptr[1];
    }

    std::cerr << "Failed to parse JPEG header: " << filePath << std::endl;
    return false;
}
