//
// Created by Administrator on 2025/4/29.
//

#include "extract_padding.h"

#include <cctype>
#include <cstdio>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 从字符串中提取数值
int extract_value(const char *str, int *index) {
    int value = 0;
    int sign = 1;
    // 处理负号
    if (str[*index] == '-') {
        sign = -1;
        (*index)++;
    }
    // 跳过非数字字符
    while (str[*index] != '\0' && (str[*index] < '0' || str[*index] > '9')) {
        (*index)++;
    }
    // 提取数字字符直到字符串结束或遇到非数字字符
    while (str[*index] != '\0' && str[*index] >= '0' && str[*index] <= '9') {
        value = value * 10 + (str[*index] - '0');
        (*index)++;
    }
    return sign * value;
}

// 从 padding 字符串中提取四个值到结构体
void extract_padding_values(const char *padding, Padding *p) {
    int index = 0;
    int count = 0;
    int temp_values[4];

    while (padding[index] != '\0' && count < 4) {
        // 跳过连续空格
        while (padding[index] == ' ') {
            index++;
        }
        if (padding[index] != '\0') {
            temp_values[count] = extract_value(padding, &index);
            count++;
        }
    }

    // 根据值的数量扩展到四个值
    switch (count) {
        case 1:
            p->top = p->right = p->bottom = p->left = temp_values[0];
            break;
        case 2:
            p->top = p->bottom = temp_values[0];
            p->right = p->left = temp_values[1];
            break;
        case 3:
            p->top = temp_values[0];
            p->right = p->left = temp_values[1];
            p->bottom = temp_values[2];
            break;
        case 4:
            p->top = temp_values[0];
            p->right = temp_values[1];
            p->bottom = temp_values[2];
            p->left = temp_values[3];
            break;
        default:
            p->top = p->right = p->bottom = p->left = 0;
            break;
    }
}

// 打印提取的 padding 值
void print_padding_values(const Padding *p) {
    printf("Top: %dpx\n", p->top);
    printf("Right: %dpx\n", p->right);
    printf("Bottom: %dpx\n", p->bottom);
    printf("Left: %dpx\n", p->left);
}


// 将颜色字符串转换为十六进制值
unsigned int color_string_to_hex(const char *color_str) {
    unsigned int color = 0;
    if (color_str == NULL) {
        // 简化错误信息
        fprintf(stderr, "格式错误: 颜色字符串为 NULL，使用默认 0x%06x\n", DEFAULT_COLOR);
        return DEFAULT_COLOR;
    }
    if (color_str[0] == '0' && (color_str[1] == 'x' || color_str[1] == 'X')) {
        if (sscanf(color_str + 2, "%x", &color) != 1) {
            // 简化错误信息
            fprintf(stderr, "格式错误: '%s' ，使用默认 0x%06x\n", color_str, DEFAULT_COLOR);
            return DEFAULT_COLOR;
        }
    } else if (color_str[0] == '#') {
        if (sscanf(color_str + 1, "%x", &color) != 1) {
            // 简化错误信息
            fprintf(stderr, "格式错误: '%s' ，使用默认 0x%06x\n", color_str, DEFAULT_COLOR);
            return DEFAULT_COLOR;
        }
    } else {
        // 简化错误信息
        fprintf(stderr, "格式错误: '%s' ，使用默认 0x%06x\n", color_str, DEFAULT_COLOR);
        return DEFAULT_COLOR;
    }
    return color;
}

// 辅助函数：将字符串转换为小写
static void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char) str[i]);
    }
}

// 根据字符串查找对应的枚举值
lv_align_t get_lv_align(const char *str) {
    char temp[100];
    strcpy(temp, str);
    str_to_lower(temp);
    const char *align_names[] = {
        "default",
        "top_left",
        "top_mid",
        "top_right",
        "bottom_left",
        "bottom_mid",
        "bottom_right",
        "left_mid",
        "right_mid",
        "center",
        "out_top_left",
        "out_top_mid",
        "out_top_right",
        "out_bottom_left",
        "out_bottom_mid",
        "out_bottom_right",
        "out_left_top",
        "out_left_mid",
        "out_left_bottom",
        "out_right_top",
        "out_right_mid",
        "out_right_bottom"
    };
    for (int i = 0; i < sizeof(align_names) / sizeof(align_names[0]); i++) {
        if (strcmp(temp, align_names[i]) == 0) {
            return (lv_align_t) i;
        }
    }
    // 如果没有匹配到，返回默认值
    return LV_ALIGN_DEFAULT;
}


// 从可能包含空格等的 "xxxpx" 格式字符串中提取数值
lv_coord_t get_lv_px_value(const char *str) {
    int value = 0;
    char *endptr;
    const char *start = str;

    // 跳过前导空格
    while (isspace((unsigned char) *start)) {
        start++;
    }
    // 转换字符串为整数
    value = strtol(start, &endptr, 10);
    // 跳过尾部空格
    while (isspace((unsigned char) *endptr)) {
        endptr++;
    }
    // 检查转换是否成功，并且后面紧跟 "px"
    if (endptr != start && strcmp(endptr, "px") == 0
        || endptr != start && strcmp(endptr, "PX") == 0
        || endptr != start && strcmp(endptr, "vh") == 0
    ) {
        return value;
    }
    printf("格式错误: '%s' ，使用默认 0\n", str);
    // 格式不匹配，返回 0
    return 0;
}


int padding_main() {
    // 测试不同情况
    char padding1[] = "10px";
    char padding2[] = "10px 20px";
    char padding3[] = "10px 20px 30px";
    char padding4[] = "10px20px10px20px";
    char padding5[] = "-10px -20px -30px -40px";
    char padding6[] = "  10px   20px  30px  40px  ";

    Padding p;

    printf("Testing padding1: %s\n", padding1);
    extract_padding_values(padding1, &p);
    print_padding_values(&p);

    printf("\nTesting padding2: %s\n", padding2);
    extract_padding_values(padding2, &p);
    print_padding_values(&p);

    printf("\nTesting padding3: %s\n", padding3);
    extract_padding_values(padding3, &p);
    print_padding_values(&p);

    printf("\nTesting padding4: %s\n", padding4);
    extract_padding_values(padding4, &p);
    print_padding_values(&p);

    printf("\nTesting padding5: %s\n", padding5);
    extract_padding_values(padding5, &p);
    print_padding_values(&p);

    printf("\nTesting padding6: %s\n", padding6);
    extract_padding_values(padding6, &p);
    print_padding_values(&p);

    return 0;
}
