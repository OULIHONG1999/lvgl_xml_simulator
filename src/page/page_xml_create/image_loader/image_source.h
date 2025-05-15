//
// Created by Administrator on 2025/5/15.
//

#ifndef IMAGE_SOURCE_H
#define IMAGE_SOURCE_H

#include "image_resource.h"

class ImageSource {
public:
    virtual ~ImageSource() = default;
    virtual bool LoadImage(const std::string& path, ImageResource& resource) = 0;
};

#endif // IMAGE_SOURCE_H
