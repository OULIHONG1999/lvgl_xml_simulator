#include "forest.h"

// 创建新的森林
Forest* create_forest() {
    Forest* forest = (Forest*)malloc(sizeof(Forest));
    forest->trees = NULL;
    forest->tree_count = 0;
    forest->capacity = 0;
    return forest;
}

// 向森林中添加树
void add_tree_to_forest(Forest* forest, TreeNode* tree) {
    if (forest->tree_count >= forest->capacity) {
        forest->capacity = forest->capacity == 0 ? 1 : forest->capacity * 2;
        forest->trees = (TreeNode**)realloc(forest->trees, sizeof(TreeNode*) * forest->capacity);
    }
    forest->trees[forest->tree_count++] = tree;
}

// 释放森林的内存
void free_forest(Forest* forest) {
    if (forest == NULL) return;
    for (int i = 0; i < forest->tree_count; i++) {
        free_tree(forest->trees[i]);
    }
    free(forest->trees);
    free(forest);
}

// 遍历森林
void traverse_forest(Forest* forest, void (*callback)(TreeData*)) {
    if (forest == NULL) return;
    for (int i = 0; i < forest->tree_count; i++) {
        printf("Tree %d:\n", i + 1);
        traverse_tree(forest->trees[i], callback);
    }
}    