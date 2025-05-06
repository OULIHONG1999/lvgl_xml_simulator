//
// Created by Administrator on 2025/5/5.
//

#include "res_tool.h"
#include "SDL_image.h"
#include <cstdio>
#include <cstdlib>

bool res_read_file(const char *file_name, unsigned char **buffer, unsigned int *size) {
    FILE *fp = fopen(file_name, "rb");
    if (fp == nullptr) {
        return false; // 文件打开失败
    }

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *buffer = (unsigned char *) malloc(*size);
    if (*buffer == nullptr) {
        fclose(fp);
        return false; // 内存分配失败
    }

    size_t read_size = fread(*buffer, 1, *size, fp);
    fclose(fp);

    if (read_size != *size) {
        free(*buffer); // 读取失败时释放内存
        *buffer = nullptr;
        return false;
    }

    return true;
}

// 新增内存释放函数
void res_free_buffer(char *buffer) {
    if (buffer != nullptr) {
        free(buffer);
    }
}


extern void read_image_test();

int sdl2_main() {
    read_image_test();
    return 0;
#if 0
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Image Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800,
                                          600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 使用 fopen 打开图片文件
    FILE *file = fopen("sample.png", "rb");
    if (file == NULL) {
        printf("Could not open image file!\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 读取文件内容到内存
    char *buffer = (char *) malloc(fileSize);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    fread(buffer, 1, fileSize, file);
    fclose(file);

    // 使用 SDL_RWFromMem 创建 SDL_RWops
    SDL_RWops *rw = SDL_RWFromMem(buffer, fileSize);
    if (rw == NULL) {
        printf("Could not create SDL_RWops!\n");
        free(buffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 加载图片
    SDL_Surface *surface = IMG_Load_RW(rw, 1);
    if (surface == NULL) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_RWclose(rw);
        free(buffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 创建纹理
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_RWclose(rw);
        free(buffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 释放表面
    SDL_FreeSurface(surface);

    // 清除渲染器
    SDL_RenderClear(renderer);

    // 复制纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // 呈现渲染器
    SDL_RenderPresent(renderer);

    // 事件循环
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // 释放资源
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
#endif
}
