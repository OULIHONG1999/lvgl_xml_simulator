//
// Created by Administrator on 2025/5/15.
//

#ifndef JPG_IMAGE_LOADER_H
#define JPG_IMAGE_LOADER_H

#include "../image_loader.h"

class JPGImageLoader : public ImageLoader {
public:
    bool LoadImage(const std::string& filePath, ImageResource& resource) override;
};

#endif // JPG_IMAGE_LOADER_H
