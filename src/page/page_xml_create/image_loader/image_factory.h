//
// Created by Administrator on 2025/5/15.
//

#ifndef IMAGE_FACTORY_H
#define IMAGE_FACTORY_H

#include <memory>
#include <optional>
#include <string>
#include "image_loader.h"
#include "lvgl/src/draw/lv_img_buf.h"

class ImageFactory {
public:
    static std::unique_ptr<ImageLoader> CreateLoader(const std::string& filePath);
};

std::optional<ImageResource> loader_image(const std::string &filePath, ImageResource &res) ;
#endif // IMAGE_FACTORY_H

