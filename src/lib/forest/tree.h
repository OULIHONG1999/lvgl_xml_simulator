#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "tree_data.h"

// 定义多叉树节点结构
typedef struct TreeNode {
    TreeData *data;
    struct TreeNode **children;
    int child_count;
    int capacity;
} TreeNode;

// 创建新的树节点
TreeNode *create_node(TreeData *data);

TreeNode *tree_create_node(TreeData *data);

// 添加子节点
void add_child(TreeNode *parent, TreeNode *child);

void tree_add_child(TreeNode *parent, TreeNode *child);

// 释放树的内存
void free_tree(TreeNode *root);

// 遍历树
void traverse_tree(TreeNode *root, void (*callback)(TreeData *));

#endif
