//
// Created by Administrator on 2025/5/5.
//

#include "read_img.h"

#include <cstdint>
#include <cstdio>

#include "res_tool.h"

#include <cstring>

#include <stdio.h>
#include <string.h>

// PNG图片解析
bool parse_png_header(const unsigned char *data, int *width, int *height) {
    if (memcmp(data, "\x89PNG\r\n\x1a\n", 8) != 0) {
        printf("Invalid PNG signature\n");
        return false;
    }
    const unsigned char *ihdr = data + 8 + 4; // 修正IHDR块的起始位置
    if (memcmp(ihdr, "IHDR", 4) != 0) {
        printf("Invalid IHDR chunk\n");
        return false;
    }
    *width = (ihdr[4] << 24) | (ihdr[5] << 16) | (ihdr[6] << 8) | ihdr[7];
    *height = (ihdr[8] << 24) | (ihdr[9] << 16) | (ihdr[10] << 8) | ihdr[11];
    return true;
}

// JPEG图片解析
bool parse_jpeg_header(const unsigned char *data, int *width, int *height) {
    if (memcmp(data, "\xff\xd8\xff", 3) != 0) return false;
    const unsigned char *ptr = data + 2;
    while (ptr < data + 1024) {
        if (*ptr++ != 0xff) continue;
        uint8_t marker = *ptr++;
        printf("Marker: 0x%02x\n", marker);
        if ((marker & 0xf0) == 0xc0 &&
            marker != 0xc4 &&
            marker != 0xc8) {
            *height = (ptr[1] << 8) | ptr[2];
            *width = (ptr[3] << 8) | ptr[4];
            return true;
        }
        ptr += (ptr[0] << 8) | ptr[1];
    }
    return false;
}


// 从数据缓冲区中查找 SOF 标记并读取图像尺寸
int parse_jpeg_header(const unsigned char *buffer, size_t buffer_size, int *width, int *height) {
    // 检查文件是否以 JPEG 标记开头
    if (buffer_size < 2 || buffer[0] != 0xFF || buffer[1] != 0xD8) {
        fprintf(stderr, "不是有效的 JPEG 文件\n");
        return -1;
    }

    size_t pos = 2;
    while (pos < buffer_size) {
        // 读取标记
        if (pos + 1 >= buffer_size) {
            fprintf(stderr, "未找到 SOF 标记\n");
            return -1;
        }

        if (buffer[pos] != 0xFF) {
            fprintf(stderr, "无效的标记\n");
            return -1;
        }

        // 检查是否为 SOF 标记
        if (buffer[pos + 1] >= 0xC0 && buffer[pos + 1] <= 0xC3) {
            // 跳过标记长度
            pos += 4;
            // 读取精度（通常为 8 位）
            pos++;
            // 读取高度
            if (pos + 1 >= buffer_size) {
                fprintf(stderr, "数据不足，无法读取高度\n");
                return -1;
            }
            *height = (buffer[pos] << 8) | buffer[pos + 1];
            pos += 2;
            // 读取宽度
            if (pos + 1 >= buffer_size) {
                fprintf(stderr, "数据不足，无法读取宽度\n");
                return -1;
            }
            *width = (buffer[pos] << 8) | buffer[pos + 1];
            return 0;
        } else {
            // 跳过标记数据
            if (pos + 3 >= buffer_size) {
                fprintf(stderr, "数据不足，无法跳过标记数据\n");
                return -1;
            }
            int length = (buffer[pos + 2] << 8) | buffer[pos + 3];
            pos += length + 2;
        }
    }

    fprintf(stderr, "未找到 SOF 标记\n");
    return -1;
}

// GIF图片解析
bool parse_gif_header(const unsigned char *data, int *width, int *height) {
    if (memcmp(data, "GIF89a", 6) != 0 && memcmp(data, "GIF87a", 6) != 0) return false;
    *width = data[6] | (data[7] << 8);
    *height = data[8] | (data[9] << 8);
    return true;
}

// 图片读取接口
bool image_read(const char *file_path, ImageData *image_data) {
    // 检查输入参数
    if (file_path == nullptr || image_data == nullptr) {
        printf("Invalid input parameters\n");
        return false;
    }

    // 读取文件内容
    if (!res_read_file(file_path, &image_data->data, &image_data->size)) {
        printf("Failed to read file: %s\n", file_path);
        return false;
    }

    // 根据文件头判断格式
    if (image_data->size >= 8) {
        if (memcmp(image_data->data, "\x89PNG\r\n\x1a\n", 8) == 0) {
            image_data->format = IMAGE_FORMAT_PNG;
        } else if (memcmp(image_data->data, "\xff\xd8\xff", 3) == 0) {
            image_data->format = IMAGE_FORMAT_JPG;
        } else if (memcmp(image_data->data, "GIF89a", 6) == 0 ||
                   memcmp(image_data->data, "GIF87a", 6) == 0) {
            image_data->format = IMAGE_FORMAT_GIF;
        } else {
            image_data->format = IMAGE_FORMAT_UNKNOWN;
            printf("Unknown image format\n");
            return false;
        }
    } else {
        image_data->format = IMAGE_FORMAT_UNKNOWN;
        printf("Image file is too small\n");
        return false;
    }

    // 根据图片格式调用对应的解析函数
    switch (image_data->format) {
        case IMAGE_FORMAT_PNG:
            if (!parse_png_header(image_data->data, &image_data->width, &image_data->height)) {
                printf("Failed to parse PNG header\n");
                return false;
            }
            printf("PNG Image size: %dx%d\n", image_data->width, image_data->height);
            break;
        case IMAGE_FORMAT_JPG:
            if (parse_jpeg_header(image_data->data, image_data->size, &image_data->width, &image_data->height)) {
                printf("Failed to parse JPEG header\n");
                return false;
            }
            printf("JPG Image size: %dx%d\n", image_data->width, image_data->height);
            break;
        case IMAGE_FORMAT_GIF:
            if (!parse_gif_header(image_data->data, &image_data->width, &image_data->height)) {
                printf("Failed to parse GIF header\n");
                return false;
            }
            printf("GIF Image size: %dx%d\n", image_data->width, image_data->height);
            break;
        default:
            printf("Unsupported image format\n");
            return false;
    }

    // 设置默认通道数
    image_data->channels = 4; // 大多数图片格式使用RGBA

    return true;
}

// 释放图片数据
void image_free(ImageData *image_data) {
    if (image_data != nullptr) {
        res_free_buffer((char *) image_data->data);
        memset(image_data, 0, sizeof(ImageData));
    }
}


void read_image_test() {
    ImageData image{};
    if (image_read("sample.png", &image)) {
        printf("PNG Image size: %dx%d\n", image.width, image.height);
        image_free(&image);
    }

    // 读一个jpg
    if (image_read("sample.jpg", &image)) {
        printf("JPG Image size: %dx%d\n", image.width, image.height);
        image_free(&image);
    }
    // 读一个gif
    if (image_read("palette.gif", &image)) {
        printf("GIF Image size: %dx%d\n", image.width, image.height);
        image_free(&image);
    }
}
