//
// Created by Administrator on 2025/5/15.
//

#include "gif_image_loader.h"
#include <fstream>
#include <iostream>

bool GIFImageLoader::LoadImage(const std::string& filePath, ImageResource& resource) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open GIF file: " << filePath << std::endl;
        return false;
    }

    resource.size = file.tellg();
    file.seekg(0, std::ios::beg);
    resource.data.resize(resource.size);

    if (!file.read(reinterpret_cast<char*>(resource.data.data()), resource.size)) {
        std::cerr << "Error: Failed to read GIF data: " << filePath << std::endl;
        return false;
    }

    resource.sourceType = "File";
    resource.type = ImageType::GIF;

    if (resource.size >= 10) {
        resource.width = resource.data[6] | (resource.data[7] << 8);
        resource.height = resource.data[8] | (resource.data[9] << 8);
    }

    return true;
}
