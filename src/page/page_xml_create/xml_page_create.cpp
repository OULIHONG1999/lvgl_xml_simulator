//
// Created by Administrator on 2025/4/26.
//

#include "xml_page_create.h"

#include <iostream>

#include "../../page_manager/page_manager.h"
#include "src/lib/forest/tree.h"
#include "src/lib/link_list/link_list.h"
#include "src/lib/tinyxml/tinyxml2.h"
#include "src/page/graphic_element/graphic_element.h"
#include "xml_button/xml_button_create.h"
#include "xml_img/xml_img_create.h"
#include "xml_label/xml_label_create.h"
#include "xml_obj/xml_obj_create.h"
#include "extract_padding.h"


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

#define LV_DEFAULT_WIDTH 170
#define LV_DEFAULT_HEIGHT 320

typedef struct {
    unsigned int text_color;
    lv_align_t text_align;
    const lv_font_t *font;
} lv_text_t;

typedef struct {
    int x;
    int y;
    int w;
    int h;
    Padding p;
    unsigned int bg_color;
    int border_width;
    unsigned int border_color;
    int radius;
} lv_base_t;

typedef struct {
    lv_base_t base;
    lv_style_t style;
    lv_text_t text;
} lv_data_t;

typedef struct {
    std::string *id;
    lv_obj_t *lv_obj;
    lv_data_t *lv_data;
} activity_t;


typedef struct {
    std::string *text;
    lv_obj_t *lv_obj;
    lv_data_t *lv_data;
} text_t;

// 提取等式右边的字符串
std::string extractRightString(const std::string &equation) {
    size_t equalPos = equation.find("==");
    if (equalPos == std::string::npos) {
        return ""; // 没有找到 == 符号，等式无效
    }

    // 提取左边部分
    std::string left = equation.substr(0, equalPos);
    // 去除左边部分可能存在的前后空格
    left.erase(0, left.find_first_not_of(" \t"));
    left.erase(left.find_last_not_of(" \t") + 1);

    // 检查左边是否为 self.id
    if (left != "self.id") {
        return ""; // 左边不是 self.id
    }

    // 提取右边部分
    std::string right = equation.substr(equalPos + 2);
    // 去除右边部分可能存在的前后空格
    right.erase(0, right.find_first_not_of(" \t"));
    right.erase(right.find_last_not_of(" \t") + 1);

    return right;
}

static std::string activity_id = "main";
static ListNode *ls_head = nullptr;

lv_obj_t *lv_create_activity(const XMLElement *element, activity_t *activity) {
    lv_obj_t *obj = lv_obj_create(nullptr);
    lv_obj_set_size(obj, LV_DEFAULT_WIDTH, LV_DEFAULT_HEIGHT);

    std::string id = element->Attribute("id");
    std::string if_ = element->Attribute("if");

    // 判断if条件是否满足
    if (std::strcmp(id.c_str(), activity_id.c_str()) == 0) {
        // 加载lv_obj
        lv_disp_load_scr(obj);
    }
    activity->lv_obj = obj;
    return obj;
}

// 写个接口，判断两个字符串是否相对
bool isRelative(const char *str1, const char *str2) {
    if (std::strcmp(str1, str2) == 0) {
        return true;
    }
    return false;
}

// 初始化lv_data_t 的内存接口
lv_data_t *lv_data_init() {
    lv_data_t *lv_data = (lv_data_t *) malloc(sizeof(lv_data_t));
    lv_style_init(&lv_data->style);
    return lv_data;
}

const char *find_element_attribute(const XMLElement *element, const char *attr_name) {
    const XMLAttribute *attribute = element->FindAttribute(attr_name);
    if (attribute) {
        return attribute->Value();
    }
    return "";
}

// 创建style
lv_style_t *lv_create_style(const XMLElement *element, lv_data_t *lv_data) {
    if (lv_data == nullptr)
        return nullptr;
    // 拿到style
    lv_style_t *style = &lv_data->style; // 已经过初始化
    // 提取padding
    const char *padding = find_element_attribute(element, "padding");
    if (std::strcmp(padding, "") != 0) {
        extract_padding_values(padding, &lv_data->base.p);
        print_padding_values(&lv_data->base.p);
    }

    // 添加单个的pandding
    const char *top = find_element_attribute(element, "top");
    if (std::strcmp(top, "") != 0) {
        lv_data->base.p.top = get_lv_px_value(top);
        lv_style_set_pad_top(style, lv_data->base.p.top);
    }
    const char *right = find_element_attribute(element, "right");
    if (std::strcmp(right, "") != 0) {
        lv_data->base.p.right = get_lv_px_value(right);
        lv_style_set_pad_right(style, lv_data->base.p.right);
    }
    const char *bottom = find_element_attribute(element, "bottom");
    if (std::strcmp(bottom, "") != 0) {
        lv_data->base.p.bottom = get_lv_px_value(bottom);
        lv_style_set_pad_bottom(style, lv_data->base.p.bottom);
    }
    const char *left = find_element_attribute(element, "left");
    if (std::strcmp(left, "") != 0) {
        lv_data->base.p.left = get_lv_px_value(left);
        lv_style_set_pad_left(style, lv_data->base.p.left);
    }

    // 获取bg_color
    const char *bg_color = find_element_attribute(element, "bg-color");
    if (std::strcmp(bg_color, "") != 0) {
        lv_data->base.bg_color = color_string_to_hex(bg_color);
        lv_style_set_bg_color(style, lv_color_hex(lv_data->base.bg_color));
    }

    // 获取字体颜色
    const char *font_color = find_element_attribute(element, "font-color");
    printf("font_color: %s\n", font_color);
    if (std::strcmp(font_color, "") != 0) {
        lv_data->text.text_color = color_string_to_hex(font_color);
        lv_style_set_text_color(style, lv_color_hex(lv_data->text.text_color));
    }

    const char *border_color = find_element_attribute(element, "border-color");
    if (std::strcmp(border_color, "") != 0) {
        lv_data->base.border_color = color_string_to_hex(border_color);
        lv_style_set_border_color(style, lv_color_hex(lv_data->base.border_color));
    }
    const char *border_width = find_element_attribute(element, "border-width");
    if (std::strcmp(border_width, "") != 0) {
        lv_data->base.border_width = get_lv_px_value(border_width);
        lv_style_set_border_width(style, lv_data->base.border_width);
    }
    const char *text_color = find_element_attribute(element, "text-color");
    if (std::strcmp(text_color, "") != 0) {
        lv_data->text.text_color = color_string_to_hex(text_color);
        lv_style_set_text_color(style, lv_color_hex(lv_data->text.text_color));
    }
    const char *text_font = find_element_attribute(element, "font");
    if (std::strcmp(text_font, "") != 0) {
        // TODO: 加载字体
    }
    const char *text_size = find_element_attribute(element, "font-size");
    if (std::strcmp(text_size, "") != 0) {
        lv_data->text.font = findFontByName(text_size);
        lv_style_set_text_font(style, lv_data->text.font);
    } else {
        lv_style_set_text_font(style, findFontBySize(12));
    }
    const char *text_align = find_element_attribute(element, "text-align");
    if (std::strcmp(text_align, "") != 0) {
        lv_data->text.text_align = get_lv_align(text_align);
        lv_style_set_text_align(style, lv_data->text.text_align);
    }

    return style;
}

lv_obj_t *lv_create_label(const XMLElement *element, lv_obj_t *parent, text_t *text) {
    const char *value = find_element_attribute(element, "value");

    // 分配text中的data内存
    text->lv_data = lv_data_init();
    // 初始化stylelv_data_t
    lv_create_style(element, text->lv_data);
    lv_obj_t *obj = lv_label_create(parent);
    lv_label_set_text(obj, value);
    // 设置style
    lv_obj_add_style(obj, &text->lv_data->style, LV_PART_MAIN);
    if (text != nullptr)
        text->lv_obj = obj;
    return obj;
}

int lv_free_activity(void *) {
    return 0;
}

// 递归遍历 XML 元素
void traverseElement(const XMLElement *element, int depth = 0) {
    if (!element) return;

    // 打印元素名称
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "Element: " << element->Name() << std::endl;
    if (isRelative(element->Name(), "activity")) {
        activity_t *activity = (activity_t *) malloc(sizeof(activity_t));
        lv_create_activity(element, activity);
        appendNode(ls_head, activity);
    }

    if (isRelative(element->Name(), "text")) {
        int node_count = getNodeCount(ls_head);
        ListNode *node = findNode(ls_head, node_count - 1);
        if (node) {
            activity_t *activity = (activity_t *) node->data;
            text_t *text = (text_t *) malloc(sizeof(text_t));
            lv_create_label(element, activity->lv_obj, text);
        }
    }


    const XMLAttribute *attribute = element->FirstAttribute();
    while (attribute) {
        for (int i = 0; i < depth + 1; ++i) {
            std::cout << "  ";
        }
        std::cout << "Attribute: " << attribute->Name() << " = " << attribute->Value() << std::endl;
        attribute = attribute->Next();
    }

    // 打印元素文本内容
    const char *text = element->GetText();
    if (text && std::string(text).length() > 0) {
        for (int i = 0; i < depth + 1; ++i) {
            std::cout << "  ";
        }
        std::cout << "Text: " << text << std::endl;
    }

    // 递归遍历子元素
    const XMLElement *child = element->FirstChildElement();
    while (child) {
        traverseElement(child, depth + 1);
        child = child->NextSiblingElement();
    }
}

void xml_page_create(const char *xml_path) {
    // 加载 XML 文件
    printf("xml_path: %s\n", xml_path);
    XMLDocument doc;
    if (doc.LoadFile(xml_path) != XML_SUCCESS) {
        std::cerr << "无法加载 XML 文件: " << doc.ErrorStr() << std::endl;
        return;
    }

    const XMLElement *root = doc.RootElement();
    if (root) {
        traverseElement(root);
    }
}


void xml_page_create0(const char *xml_path) {
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
