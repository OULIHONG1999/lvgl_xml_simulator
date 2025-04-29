//
// Created by Administrator on 2025/4/28.
//

#ifndef LINK_LIST_H
#define LINK_LIST_H

// 定义链表节点结构体
struct ListNode {
    int index;
    void *data;
    ListNode *next;

    ListNode(int idx, void *d) : index(idx), data(d), next(nullptr) {
    }
};

void insertNode(ListNode *&head, int index, void *data);

// 新增方法：自动在末尾添加节点
void appendNode(ListNode *&head, void *data);

// 删除指定 index 的节点
void deleteNode(ListNode *&head, int index);

// 查找指定 index 的节点
ListNode *findNode(ListNode *head, int index);

// 修改指定 index 节点的数据
void modifyNode(ListNode *head, int index, void *newData);

// 打印链表中节点的 index
void printList(ListNode *head);

// 新增方法：查询当前链表有多少个节点
int getNodeCount(ListNode *head);

// 释放链表内存
void freeList(ListNode *&head);

#endif //LINK_LIST_H
