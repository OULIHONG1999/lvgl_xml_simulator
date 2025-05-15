//
// Created by Administrator on 2025/5/15.
//

#ifndef PNG_IMAGE_LOADER_H
#define PNG_IMAGE_LOADER_H

#include "../image_loader.h"

class PNGImageLoader : public ImageLoader {
public:
    bool LoadImage(const std::string& filePath, ImageResource& resource) override;
};

#endif // PNG_IMAGE_LOADER_H
