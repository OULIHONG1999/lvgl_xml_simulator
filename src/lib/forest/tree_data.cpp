#include "tree_data.h"

// 创建 TreeData
TreeData* create_tree_data(void* id, void (*free_func)(void*)) {
    TreeData* data = (TreeData*)malloc(sizeof(TreeData));
    if (data) {
        data->id = id;
        data->free_self = free_func;
    }
    return data;
}

// 释放 TreeData
void free_tree_data(TreeData* data) {
    if (data) {
        if (data->free_self) {
            data->free_self(data->id);
        }
        free(data);
    }
}