//
// Created by Administrator on 2025/5/15.
//

#include "bmp_image_loader.h"
#include <fstream>
#include <iostream>

bool BMPImageLoader::LoadImage(const std::string& filePath, ImageResource& resource) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open BMP file: " << filePath << std::endl;
        return false;
    }

    resource.size = file.tellg();
    file.seekg(0, std::ios::beg);
    resource.data.resize(resource.size);

    if (!file.read(reinterpret_cast<char*>(resource.data.data()), resource.size)) {
        std::cerr << "Error: Failed to read BMP data: " << filePath << std::endl;
        return false;
    }

    resource.sourceType = "File";
    resource.type = ImageType::BMP;

    if (resource.size >= 18) { // BMP 头部信息
        resource.width = resource.data[18] | (resource.data[19] << 8);
        resource.height = resource.data[22] | (resource.data[23] << 8);
    }

    return true;
}
