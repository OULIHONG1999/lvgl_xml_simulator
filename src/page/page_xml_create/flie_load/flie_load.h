//
// Created by Administrator on 2025/5/15.
//

#ifndef FLIE_LOAD_H
#define FLIE_LOAD_H

int flie_load(const char *file_name, unsigned char **buffer, unsigned int *size);
void flie_free(unsigned char *buffer);

#endif //FLIE_LOAD_H
