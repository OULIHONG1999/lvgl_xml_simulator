//
// Created by Administrator on 2025/5/5.
//

#ifndef RES_TOOL_H
#define RES_TOOL_H

#ifdef __cplusplus
// 仅对函数声明使用extern "C"
extern "C" int sdl2_main();
#endif

bool res_read_file(const char *file_name, unsigned char **buffer, unsigned int *size);

void res_free_buffer(char *buffer);

#endif //RES_TOOL_H
