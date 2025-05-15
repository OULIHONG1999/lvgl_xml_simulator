//
// Created by Administrator on 2025/5/15.
//

#ifndef GIF_IMAGE_LOADER_H
#define GIF_IMAGE_LOADER_H

#include "../image_loader.h"

class GIFImageLoader : public ImageLoader {
public:
    bool LoadImage(const std::string& filePath, ImageResource& resource) override;
};

#endif // GIF_IMAGE_LOADER_H
