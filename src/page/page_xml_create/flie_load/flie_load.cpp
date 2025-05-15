//
// Created by Administrator on 2025/5/15.
//

#include "flie_load.h"
#include <stdio.h>
#include <stdlib.h>

// 读取文件内容到缓冲区
int flie_load(const char *file_name, unsigned char **buffer, unsigned int *size) {
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        return 0; // 文件打开失败
    }

    // 获取文件大小
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return 0;
    }

    long fileSize = ftell(fp);
    if (fileSize == -1) {
        fclose(fp);
        return 0;
    }
    *size = (unsigned int)fileSize;
    rewind(fp); // 归位文件指针

    // 分配内存
    *buffer = (unsigned char *)malloc(*size);
    if (*buffer == NULL) {
        fclose(fp);
        return 0; // 内存分配失败
    }

    // 读取文件内容
    size_t read_size = fread(*buffer, 1, *size, fp);
    fclose(fp);

    // 读取失败时释放内存
    if (read_size != *size) {
        free(*buffer);
        *buffer = NULL;
        return 0;
    }

    return 1;
}

// 释放文件缓冲区
void file_free(unsigned char *buffer) {
    if (buffer != NULL) {
        free(buffer);
    }
}