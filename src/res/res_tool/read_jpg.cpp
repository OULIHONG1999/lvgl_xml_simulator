//
// Created by Administrator on 2025/5/6.
//

#include "read_jpg.h"
#include <stdio.h>
#include <stdlib.h>

// 从数据缓冲区中查找 SOF 标记并读取图像尺寸
int read_jpeg_dimensions_from_buffer(unsigned char *buffer, size_t buffer_size, int *width, int *height) {

    // 打印 buffer
    printf("buffer: ");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

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

#include <stdio.h>
#include <stdlib.h>

// 上面的 read_jpeg_dimensions_from_buffer 函数

int main() {
    const char *file_name = "E:\\work_space\\LVGL\\SimulatorV8.3\\lv_simulator_show_xml\\bin\\sample.jpg";
    unsigned char *buffer;
    unsigned int size;
    FILE *fp = fopen(file_name, "rb");
    if (fp == nullptr) {
        return false; // 文件打开失败
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

   buffer = (unsigned char *) malloc(size);
    if (buffer == nullptr) {
        fclose(fp);
        return false; // 内存分配失败
    }

    size_t read_size = fread(buffer, 1, size, fp);
    fclose(fp);

    if (read_size != size) {
        free(buffer); // 读取失败时释放内存
        buffer = nullptr;
        return false;
    }


    int width, height;
    if (read_jpeg_dimensions_from_buffer(buffer, size, &width, &height) == 0) {
        printf("图像宽度: %d 像素\n", width);
        printf("图像高度: %d 像素\n", height);
    }

    return 0;
}