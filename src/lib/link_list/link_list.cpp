#include <iostream>
#include "link_list.h"


// 插入节点到链表尾部
void insertNode(ListNode*& head, int index, void* data) {
    ListNode* newNode = new ListNode(index, data);
    if (!head) {
        head = newNode;
        return;
    }
    ListNode* temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// 新增方法：自动在末尾添加节点
void appendNode(ListNode*& head, void* data) {
    int newIndex = 0;
    if (head) {
        ListNode* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        newIndex = temp->index + 1;
    }
    insertNode(head, newIndex, data);
}

// 删除指定 index 的节点
void deleteNode(ListNode*& head, int index) {
    if (!head) return;
    if (head->index == index) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
        return;
    }
    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr) {
        if (curr->index == index) {
            prev->next = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// 查找指定 index 的节点
ListNode* findNode(ListNode* head, int index) {
    ListNode* temp = head;
    while (temp) {
        if (temp->index == index) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// 修改指定 index 节点的数据
void modifyNode(ListNode* head, int index, void* newData) {
    ListNode* node = findNode(head, index);
    if (node) {
        node->data = newData;
    }
}

// 打印链表中节点的 index
void printList(ListNode* head) {
    ListNode* temp = head;
    while (temp) {
        std::cout << "Index: " << temp->index << std::endl;
        temp = temp->next;
    }
}

// 新增方法：查询当前链表有多少个节点
int getNodeCount(ListNode* head) {
    int count = 0;
    ListNode* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// 释放链表内存
void freeList(ListNode*& head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int test_node_main() {
    ListNode* head = nullptr;
    int num1 = 100;
    int num2 = 200;
    int num3 = 300;

    // 使用 appendNode 方法添加节点
    appendNode(head, &num1);
    appendNode(head, &num2);
    appendNode(head, &num3);
    std::cout << "插入节点后的链表：" << std::endl;
    printList(head);

    // 查询链表节点数量
    int nodeCount = getNodeCount(head);
    std::cout << "当前链表节点数量: " << nodeCount << std::endl;

    // 删除节点
    deleteNode(head, 1);
    std::cout << "删除 index 为 1 的节点后的链表：" << std::endl;
    printList(head);

    // 再次查询链表节点数量
    nodeCount = getNodeCount(head);
    std::cout << "删除节点后链表节点数量: " << nodeCount << std::endl;

    // 查找节点
    ListNode* found = findNode(head, 2);
    if (found) {
        std::cout << "找到了 index 为 2 的节点" << std::endl;
    } else {
        std::cout << "未找到 index 为 2 的节点" << std::endl;
    }

    // 修改节点数据
    int newNum = 400;
    modifyNode(head, 0, &newNum);
    std::cout << "修改 index 为 0 的节点数据后的链表：" << std::endl;
    printList(head);

    // 释放链表内存
    freeList(head);

    return 0;
}    