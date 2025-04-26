#ifndef FOREST_H
#define FOREST_H

#include "tree.h"

// 定义森林结构
typedef struct {
    TreeNode** trees;
    int tree_count;
    int capacity;
} Forest;

// 创建新的森林
Forest* create_forest();
// 向森林中添加树
void add_tree_to_forest(Forest* forest, TreeNode* tree);
// 释放森林的内存
void free_forest(Forest* forest);
// 遍历森林
void traverse_forest(Forest* forest, void (*callback)(TreeData*));

#endif    