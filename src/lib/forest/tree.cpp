#include "tree.h"

// 创建新的树节点
TreeNode* create_node(TreeData* data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->children = NULL;
    node->child_count = 0;
    node->capacity = 0;
    return node;
}
// 创建新的树节点
TreeNode* tree_create_node(TreeData* data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->children = NULL;
    node->child_count = 0;
    node->capacity = 0;
    return node;
}

// 添加子节点
void add_child(TreeNode* parent, TreeNode* child) {
    if (parent->child_count >= parent->capacity) {
        parent->capacity = parent->capacity == 0 ? 1 : parent->capacity * 2;
        parent->children = (TreeNode**)realloc(parent->children, sizeof(TreeNode*) * parent->capacity);
    }
    parent->children[parent->child_count++] = child;
}
// 添加子节点
void tree_add_child(TreeNode* parent, TreeNode* child) {
    if (parent->child_count >= parent->capacity) {
        parent->capacity = parent->capacity == 0 ? 1 : parent->capacity * 2;
        parent->children = (TreeNode**)realloc(parent->children, sizeof(TreeNode*) * parent->capacity);
    }
    parent->children[parent->child_count++] = child;
}

// 释放树的内存
void free_tree(TreeNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }
    free_tree_data(root->data);
    free(root->children);
    free(root);
}

// 遍历树
void traverse_tree(TreeNode* root, void (*callback)(TreeData*)) {
    if (root == NULL) return;
    callback(root->data);
    for (int i = 0; i < root->child_count; i++) {
        traverse_tree(root->children[i], callback);
    }
}    