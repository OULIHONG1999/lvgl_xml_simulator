//
// Created by Administrator on 2025/4/26.
//

#include "xml_page_create.h"
#include "../../page_manager/page_manager.h"
#include "src/lib/tinyxml/tinyxml2.h"
#include "src/page/graphic_element/graphic_element.h"
#include "xml_button/xml_button_create.h"
#include "xml_img/xml_img_create.h"
#include "xml_label/xml_label_create.h"
#include "xml_obj/xml_obj_create.h"


// 复制字符串
char *copy_string(const char *src) {
    if (src == nullptr) {
        return nullptr;
    }
    char *dest = (char *) malloc(strlen(src) + 1);
    strcpy(dest, src);
    return dest;
}


lv_obj_data_t *set_lv_obj_user_data(GraphicElementType type, const XMLElement *element) {
    lv_obj_data_t *data = (lv_obj_data_t *) malloc(sizeof(lv_obj_data_t));
    data->type = type;
    data->name = copy_string(element->Name());
    return data;
}

void lv_obj_set_base_element(lv_obj_t *obj, GraphicElement *ge) {
    lv_obj_set_size(obj, static_cast<lv_coord_t>(ge->w), static_cast<lv_coord_t>(ge->h));
    lv_obj_set_pos(obj, static_cast<lv_coord_t>(ge->x), static_cast<lv_coord_t>(ge->y));
}


void xml_page_create(const char *xml_path) {
    // 创建 XML 文档
    XMLDocument doc;
    doc.LoadFile(xml_path);

    // 访问head子节点
    const XMLElement *page = doc.RootElement();
    if (page == nullptr) {
        printf("Failed to load XML file.\n");
        return;
    }
    printf("page_name: %s\n", page->Name());
    GraphicElement *ge_page = xml_obj_create_element(page);

    // 创建页面
    lv_obj_t *page_obj = lv_xml_obj_create(nullptr, ge_page, page);

    lv_disp_load_scr(page_obj);
    // lv_scr_load_anim(page_obj, LV_SCR_LOAD_ANIM_FADE_OUT, 200, 10, false);

    const XMLElement *child[10];
    for (int i = 0; i < 10; ++i) {
        if (i == 0)
            child[i] = page->FirstChildElement();
        else
            child[i] = child[i - 1]->NextSiblingElement();

        if (child[i] == nullptr) {
            break;
        }
        const char *child_name = child[i]->Name();
        printf("child_name: %s\n", child_name);
        if (strcmp(child_name, "head") == 0 || strcmp(child_name, "belly") == 0) {
            GraphicElement *ge_head = xml_obj_create_element(child[i]);
            lv_obj_t *head_obj = lv_xml_obj_create(page_obj, ge_head, child[i]);
            // 向内部遍历元素
            const XMLElement *child_c[10];
            for (int j = 0; j < 10; ++j) {
                if (j == 0)
                    child_c[j] = child[i]->FirstChildElement();
                else
                    child_c[j] = child_c[j - 1]->NextSiblingElement();

                if (child_c[j] == nullptr) {
                    break;
                }

                const char *child_c_name = child_c[j]->Name();
                printf("child_c_name: %s\n", child_c_name);
                // 判断child_c_name是否为img
                if (strcmp(child_c_name, "img") == 0) {
                    GraphicElement *ge_img = xml_img_create_element(child_c[j]);
                    ge_img->lv_obj = static_cast<void *>(lv_xml_img_create(
                        head_obj, ge_img, child_c[j]));
                }
                // 判断child_c_name是否为label
                if (strcmp(child_c_name, "label") == 0) {
                    GraphicElement *ge_label = xml_label_create_element(child_c[j]);
                    ge_label->lv_obj = lv_xml_label_create(
                        head_obj, ge_label, child_c[j]);
                }
                // 判断child_c_name是否为button
                if (strcmp(child_c_name, "button") == 0) {
                    GraphicElement *ge_button = xml_button_create_element(child_c[j]);
                    ge_button->lv_obj = lv_xml_button_create(
                        head_obj, ge_button, child_c[j]);
                }
            }
        }
    }

    printf("\n\n\n\n");
    int child_num = lv_obj_get_child_cnt(page_obj);
    for (int i = 0; i < child_num; ++i) {
        lv_obj_t *child = lv_obj_get_child(page_obj, i);
        lv_obj_data_t *lv_obj_data = (lv_obj_data_t *) lv_obj_get_user_data(child);
        printf(" %s type: %d\n", lv_obj_data->name, lv_obj_data->type);

        int num = lv_obj_get_child_cnt(child);
        // printf("child_num: %d\n", num);
        for (int j = 0; j < num; ++j) {
            lv_obj_t *child_t = lv_obj_get_child(child, j);
            lv_obj_data_t *lv_obj_data = (lv_obj_data_t *) lv_obj_get_user_data(child_t);
            if (lv_obj_data == nullptr)
                printf("child_obj[%d] 0 type: %d\n", j, -1);
            else {
                if (lv_obj_data->name != nullptr)
                    printf(" %s type: %d\n", lv_obj_data->name, lv_obj_data->type);
            }
        }
    }
}
