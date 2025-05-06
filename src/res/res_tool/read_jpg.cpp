//
// Created by Administrator on 2025/5/6.
//

#include "read_jpg.h"
#include <stdio.h>
#include <stdlib.h>

// �����ݻ������в��� SOF ��ǲ���ȡͼ��ߴ�
int read_jpeg_dimensions_from_buffer(unsigned char *buffer, size_t buffer_size, int *width, int *height) {

    // ��ӡ buffer
    printf("buffer: ");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // ����ļ��Ƿ��� JPEG ��ǿ�ͷ
    if (buffer_size < 2 || buffer[0] != 0xFF || buffer[1] != 0xD8) {
        fprintf(stderr, "������Ч�� JPEG �ļ�\n");
        return -1;
    }

    size_t pos = 2;
    while (pos < buffer_size) {
        // ��ȡ���
        if (pos + 1 >= buffer_size) {
            fprintf(stderr, "δ�ҵ� SOF ���\n");
            return -1;
        }

        if (buffer[pos] != 0xFF) {
            fprintf(stderr, "��Ч�ı��\n");
            return -1;
        }

        // ����Ƿ�Ϊ SOF ���
        if (buffer[pos + 1] >= 0xC0 && buffer[pos + 1] <= 0xC3) {
            // ������ǳ���
            pos += 4;
            // ��ȡ���ȣ�ͨ��Ϊ 8 λ��
            pos++;
            // ��ȡ�߶�
            if (pos + 1 >= buffer_size) {
                fprintf(stderr, "���ݲ��㣬�޷���ȡ�߶�\n");
                return -1;
            }
            *height = (buffer[pos] << 8) | buffer[pos + 1];
            pos += 2;
            // ��ȡ���
            if (pos + 1 >= buffer_size) {
                fprintf(stderr, "���ݲ��㣬�޷���ȡ���\n");
                return -1;
            }
            *width = (buffer[pos] << 8) | buffer[pos + 1];
            return 0;
        } else {
            // �����������
            if (pos + 3 >= buffer_size) {
                fprintf(stderr, "���ݲ��㣬�޷������������\n");
                return -1;
            }
            int length = (buffer[pos + 2] << 8) | buffer[pos + 3];
            pos += length + 2;
        }
    }

    fprintf(stderr, "δ�ҵ� SOF ���\n");
    return -1;
}

#include <stdio.h>
#include <stdlib.h>

// ����� read_jpeg_dimensions_from_buffer ����

int main() {
    const char *file_name = "E:\\work_space\\LVGL\\SimulatorV8.3\\lv_simulator_show_xml\\bin\\sample.jpg";
    unsigned char *buffer;
    unsigned int size;
    FILE *fp = fopen(file_name, "rb");
    if (fp == nullptr) {
        return false; // �ļ���ʧ��
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

   buffer = (unsigned char *) malloc(size);
    if (buffer == nullptr) {
        fclose(fp);
        return false; // �ڴ����ʧ��
    }

    size_t read_size = fread(buffer, 1, size, fp);
    fclose(fp);

    if (read_size != size) {
        free(buffer); // ��ȡʧ��ʱ�ͷ��ڴ�
        buffer = nullptr;
        return false;
    }


    int width, height;
    if (read_jpeg_dimensions_from_buffer(buffer, size, &width, &height) == 0) {
        printf("ͼ����: %d ����\n", width);
        printf("ͼ��߶�: %d ����\n", height);
    }

    return 0;
}