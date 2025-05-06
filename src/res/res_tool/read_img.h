//
// Created by Administrator on 2025/5/5.
//

#ifndef READ_IMG_H
#define READ_IMG_H

// 图片格式枚举
enum ImageFormat {
    IMAGE_FORMAT_UNKNOWN = 0,
    IMAGE_FORMAT_PNG,
    IMAGE_FORMAT_JPG,
    IMAGE_FORMAT_GIF
};

// 图片数据结构体
struct ImageData {
    ImageFormat format;
    int width;
    int height;
    int channels;
    unsigned char* data;
    unsigned int size;
};

#endif //READ_IMG_H
