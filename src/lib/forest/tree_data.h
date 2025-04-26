#ifndef TREE_DATA_H
#define TREE_DATA_H

#include <stdio.h>
#include <stdlib.h>

// 定义 TreeData 结构体
typedef struct TreeData {
    void* id;
    void (*free_self)(void*);
} TreeData;

// 创建 TreeData
TreeData* create_tree_data(void* id, void (*free_func)(void*));
// 释放 TreeData
void free_tree_data(TreeData* data);

#endif