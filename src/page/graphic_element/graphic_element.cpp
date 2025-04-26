#include "graphic_element.h"

#include "src/lib/forest/forest.h"


// 创建图形元素
GraphicElement *create_graphic_element(int x, int y, int w, int h) {
    GraphicElement *element = (GraphicElement *) malloc(sizeof(GraphicElement));
    if (element) {
        element->x = x;
        element->y = y;
        element->w = w;
        element->h = h;
    }
    return element;
}

#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10
// 创建默认图形元素
GraphicElement *create_default_graphic_element() {
    GraphicElement *element = (GraphicElement *) malloc(sizeof(GraphicElement));
    if (element) {
        element->x = 0;
        element->y = 0;
        element->w = DEFAULT_WIDTH;
        element->h = DEFAULT_HEIGHT;
    } else {
        printf("create_default_graphic_element error\n");
        return NULL;
    }
    return element;
}

// 释放图形元素
void free_graphic_element(void *element) {
    GraphicElement *e = (GraphicElement *) element;

    switch (e->type) {
        case GRAPHIC_IMG:
            if (e->img.src != nullptr) {
                free(e->img.src);
            }
            break;
        case GRAPHIC_LABEL:
            if (e->label.text != nullptr) {
                free(e->label.text);
            }
            break;
        case GRAPHIC_BUTTON:
            if (e->button.label.text != nullptr) {
                free(e->button.label.text);
            }
            break;
        default:
            break;
    }

    if (element) {
        free(element);
    }
}

// 打印图形元素信息
void print_graphic_element(GraphicElement *element) {
    if (element) {
        printf("Graphic Element: x = %d, y = %d, w = %d, h = %d\n",
               element->x, element->y, element->w, element->h);
    }
}

// 打印 TreeData 中图形元素信息的回调函数
void print_tree_data_callback(TreeData *data) {
    if (data) {
        GraphicElement *element = (GraphicElement *) data->id;
        print_graphic_element(element);
    }
}

int GraphicElement_test_main() {
    // 创建森林
    Forest *forest = create_forest();

    // 创建图形元素
    GraphicElement *element1 = create_graphic_element(10, 20, 100, 200);
    GraphicElement *element2 = create_graphic_element(30, 40, 50, 100);

    // 创建 TreeData
    TreeData *data1 = create_tree_data((void *) element1, free_graphic_element);
    TreeData *data2 = create_tree_data((void *) element2, free_graphic_element);

    // 创建树节点
    TreeNode *root1 = create_node(data1);
    TreeNode *root2 = create_node(data2);

    // 向森林中添加树
    add_tree_to_forest(forest, root1);
    add_tree_to_forest(forest, root2);

    // 遍历森林并打印图形元素信息
    traverse_forest(forest, print_tree_data_callback);

    // 释放森林的内存
    free_forest(forest);

    return 0;
}
