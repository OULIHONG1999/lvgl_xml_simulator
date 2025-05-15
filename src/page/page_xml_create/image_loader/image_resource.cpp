//
// Created by Administrator on 2025/5/15.
//

#include "image_resource.h"

void ImageResource::release() {
    data.clear();
    size = 0;
}
