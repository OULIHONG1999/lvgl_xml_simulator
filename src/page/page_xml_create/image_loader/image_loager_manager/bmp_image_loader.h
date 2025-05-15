//
// Created by Administrator on 2025/5/15.
//

#ifndef BMP_IMAGE_LOADER_H
#define BMP_IMAGE_LOADER_H

#include "../image_loader.h"

class BMPImageLoader : public ImageLoader {
public:
    bool LoadImage(const std::string& filePath, ImageResource& resource) override;
};

#endif // BMP_IMAGE_LOADER_H
