//
// Created by Administrator on 2025/5/15.
//

#include "image_factory.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <fstream>

#include "flie_loader/FirmwareResourceManager.h"
#include "flie_loader/NetworkImageLoader.h"
#include "image_loader_manager/bmp_image_loader.h"
#include "image_loader_manager/gif_image_loader.h"
#include "image_loader_manager/jpg_image_loader.h"
#include "image_loader_manager/png_image_loader.h"

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

/**
 * @brief 根据路径自动判断图片来源并加载
 * @param path 输入的图片路径
 *             - 网络URL: 以 http://、https://、ftp:// 开头
 *             - 固件资源: 约定 res:// 前缀 或固件资源表存在
 *             - 本地文件: 其他情况视为本地文件路径
 * @param res  输出的图片资源对象（宽、高、数据等）
 * @return 成功返回 ImageResource，失败返回 std::nullopt
 */
std::optional<ImageResource> loader_image_auto(const std::string &path, ImageResource &res) {
    using namespace std::literals;

    // 1. 网络 URL 检测
    bool isHttp = path.rfind("http://", 0) == 0;
    bool isHttps = path.rfind("https://", 0) == 0;
    bool isFtp = path.rfind("ftp://", 0) == 0;

    if (isHttp || isHttps || isFtp) {
        std::cout << "[Loader] Detected network URL: " << path << std::endl;

        if (NetworkImageLoader::LoadFromUrl(path, res)) {
            res.sourceType = "network";
            std::cout << "[Loader] Network image loaded: "
                    << res.width << "x" << res.height
                    << ", Size: " << res.size << " bytes" << std::endl;
            return res;
        }

        std::cerr << "[Loader] Failed to load image from URL: " << path << std::endl;
        return std::nullopt;
    }

    // 2. 固件资源检测（前缀匹配或查询资源表）
    if (path.rfind("res://", 0) == 0 || FirmwareResourceManager::Exists(path)) {
        std::cout << "[Loader] Detected firmware resource: " << path << std::endl;

        if (FirmwareResourceManager::LoadImage(path, res)) {
            res.sourceType = "firmware";
            std::cout << "[Loader] Firmware image loaded: "
                    << res.width << "x" << res.height
                    << ", Size: " << res.size << " bytes" << std::endl;
            return res;
        }

        std::cerr << "[Loader] Failed to load image from firmware: " << path << std::endl;
        return std::nullopt;
    }

    // 3. 本地文件检测（绝对/相对路径）
    if (std::filesystem::exists(path)) {
        std::cout << "[Loader] Detected local file: " << path << std::endl;

        auto loader = ImageFactory::CreateLoader(path);
        if (!loader) {
            std::cerr << "[Loader] Unsupported image format (local): " << path << std::endl;
            return std::nullopt;
        }

        if (loader->LoadImage(path, res)) {
            res.sourceType = "local";
            std::cout << "[Loader] Local image loaded: "
                    << res.width << "x" << res.height
                    << ", Size: " << res.size << " bytes" << std::endl;
            return res;
        }

        std::cerr << "[Loader] Image load failed (local): " << path << std::endl;
        return std::nullopt;
    }

    // 4. 未知类型
    std::cerr << "[Loader] Unknown image source type: " << path << std::endl;
    return std::nullopt;
}

std::optional<ImageResource> loader_image(const std::string &filePath, ImageResource &res) {
    auto loader = ImageFactory::CreateLoader(filePath);

    if (!loader) {
        std::cerr << "Unsupported image format." << std::endl;
        return std::nullopt;
    }
    // 3. 使用对应的加载器加载图片
    if (loader) {
        if (loader->LoadImage(filePath, res)) {
            std::cout << "Loaded Image: " << res.sourceType << ", Size: " << res.size << ", Dimensions: " << res.width
                    << "x" << res.height << std::endl;
            return res;
        }
    }
    std::cerr << "[Loader] Image load failed (local): " << filePath << std::endl;
    return std::nullopt;
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
