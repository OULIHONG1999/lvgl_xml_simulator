//
// Created by Administrator on 2025/5/15.
//

#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "image_resource.h"

class ImageLoader {
public:
    virtual ~ImageLoader() = default;
    virtual bool LoadImage(const std::string& filePath, ImageResource& resource) = 0;
};

#endif // IMAGE_LOADER_H
