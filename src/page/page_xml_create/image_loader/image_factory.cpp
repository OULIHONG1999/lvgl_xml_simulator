//
// Created by Administrator on 2025/5/15.
//

#include "image_factory.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <ostream>
#include <fstream>

#include "image_loager_manager/bmp_image_loader.h"
#include "image_loager_manager/gif_image_loader.h"
#include "image_loager_manager/jpg_image_loader.h"
#include "image_loager_manager/png_image_loader.h"

// **检测图片类型**
std::string detectImageType(const unsigned char *data, size_t size) {
    if (size < 8) return "Unknown"; // 数据不足，无法识别

    if (memcmp(data, "\x89PNG\r\n\x1a\n", 8) == 0) return "PNG"; // PNG
    if (memcmp(data, "\xff\xd8\xff", 3) == 0) return "JPEG"; // JPEG
    if (memcmp(data, "GIF87a", 6) == 0 || memcmp(data, "GIF89a", 6) == 0) return "GIF"; // GIF
    if (memcmp(data, "BM", 2) == 0) return "BMP"; // BMP

    return "Unknown"; // 未知格式
}

std::string imageTypeToString(ImageType type) {
    switch (type) {
        case ImageType::PNG: return "PNG";
        case ImageType::JPEG: return "JPEG";
        case ImageType::GIF: return "GIF";
        case ImageType::BMP: return "BMP";
        default: return "Unknown";
    }
}

std::optional<ImageResource> loader_image(const std::string &filePath, ImageResource &res) {
    auto loader = ImageFactory::CreateLoader(filePath);

    if (!loader) {
        std::cerr << "Unsupported image format." << std::endl;
        return std::nullopt;
    }
    if (loader) {
        if (loader->LoadImage(filePath, res)) {
            std::cout << "Loaded Image: " << res.sourceType << ", Size: " << res.size << ", Dimensions: " << res.width
                    << "x" << res.height << std::endl;
        } else {
            std::cerr << "Image load failed." << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "Unsupported image format." << std::endl;
        return std::nullopt;
    }
    return res;
}


std::unique_ptr<ImageLoader> ImageFactory::CreateLoader(const std::string &filePath) {
    // 确保 filePath 至少有 4 个字符（如 .png）
    if (filePath.length() < 4) {
        return nullptr;
    }

    // 提取文件扩展名，并转换为小写
    std::string ext = filePath.substr(filePath.find_last_of('.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    // 打印文件扩展名（调试）
    std::cout << "File extension: " << ext << std::endl;

    // **读取文件前 8 字节数据**
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return nullptr;
    }

    unsigned char header[8] = {0};
    file.read(reinterpret_cast<char *>(header), sizeof(header));
    file.close();

    // **识别真实格式**
    std::string detectedType = detectImageType(header, sizeof(header));

    // 打印一下警告，如果文件扩展名和实际格式不匹配
    if (ext != detectedType) {
        std::cerr << "Warning: File extension does not match detected image type: " << filePath << std::endl;
    }

    std::cout << "Detected Image Type: " << detectedType << std::endl;

    // **匹配实际格式**
    if (detectedType == "PNG") {
        return std::make_unique<PNGImageLoader>();
    } else if (detectedType == "JPEG") {
        return std::make_unique<JPGImageLoader>();
    } else if (detectedType == "GIF") {
        return std::make_unique<GIFImageLoader>();
    } else if (detectedType == "BMP") {
        return std::make_unique<BMPImageLoader>();
    }

    std::cerr << "Unsupported image format: " << filePath << std::endl;
    return nullptr;
}
