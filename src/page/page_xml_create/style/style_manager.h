//
// Created by Administrator on 2025/6/10.
//

#ifndef STYLE_MANAGER_H
#define STYLE_MANAGER_H


#include "../components/base_factory.h"


class StyleManager {
public:
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string> > themeStyles;

    // **加载主题样式**
    static void LoadTheme(XMLElement *themeElem) {
        if (!themeElem) return;

        std::string themeId = themeElem->Attribute("id");
        if (themeId.empty()) return;

        std::unordered_map<std::string, std::string> attributes;
        for (const XMLAttribute *attr = themeElem->FirstAttribute(); attr; attr = attr->Next()) {
            attributes[attr->Name()] = attr->Value();
        }
        themeStyles[themeId] = attributes;
    }

    // **统一处理所有属性**
    static void ApplyAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        ApplyCommonAttributes(obj, xmlElem);
        ApplyStyleAttributes(obj, xmlElem);
        ApplyLayoutAttributes(obj, xmlElem);

        if (const char *themeId = xmlElem->Attribute("theme")) {
            ApplyThemeAttributes(obj, themeId);
        }
    }

    // **通用属性**
    static void ApplyCommonAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        if (const char *width = xmlElem->Attribute("width")) {
            lv_obj_set_width(obj, std::stoi(width));
        }
        if (const char *height = xmlElem->Attribute("height")) {
            lv_obj_set_height(obj, std::stoi(height));
        }
        if (const char *x = xmlElem->Attribute("x")) {
            lv_obj_set_x(obj, std::stoi(x));
        }
        if (const char *y = xmlElem->Attribute("y")) {
            lv_obj_set_y(obj, std::stoi(y));
        }
    }

    // **样式属性**
    static void ApplyStyleAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        if (const char *background_color = xmlElem->Attribute("background_color")) {
            lv_obj_set_style_bg_color(obj, lv_color_hex(std::stoul(background_color, nullptr, 16)), 0);
        }
        if (const char *border_color = xmlElem->Attribute("border_color")) {
            lv_obj_set_style_border_color(obj, lv_color_hex(std::stoul(border_color, nullptr, 16)), 0);
        }
    }

    static void ApplyFlexLayout(lv_obj_t *obj, XMLElement *xmlElem) {
        lv_obj_set_layout(obj, LV_LAYOUT_FLEX);

        // **解析 flex 方向**
        // **解析 flex_flow**
        if (const char *flow = xmlElem->Attribute("flex_flow")) {
            if (strcmp(flow, "row") == 0) lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
            else if (strcmp(flow, "column") == 0) lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
            else if (strcmp(flow, "row_wrap") == 0) lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP);
            else if (strcmp(flow, "column_wrap") == 0) lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN_WRAP);
        }

        // **解析 flex_align**
        if (const char *align = xmlElem->Attribute("flex_align")) {
            if (strcmp(align, "start") == 0)
                lv_obj_set_flex_align(obj,
                                      LV_FLEX_ALIGN_START,
                                      LV_FLEX_ALIGN_START,

                                      LV_FLEX_ALIGN_START);
            else if (strcmp(align, "center") == 0)
                lv_obj_set_flex_align(obj,
                                      LV_FLEX_ALIGN_CENTER,
                                      LV_FLEX_ALIGN_CENTER,
                                      LV_FLEX_ALIGN_CENTER);
            else if (strcmp(align, "end") == 0)
                lv_obj_set_flex_align(obj,
                                      LV_FLEX_ALIGN_END,
                                      LV_FLEX_ALIGN_END,
                                      LV_FLEX_ALIGN_END);
            else if (strcmp(align, "space_between") == 0)
                lv_obj_set_flex_align(obj,
                                      LV_FLEX_ALIGN_SPACE_BETWEEN,
                                      LV_FLEX_ALIGN_CENTER,
                                      LV_FLEX_ALIGN_CENTER);
            else if (strcmp(align, "space_around") == 0)
                lv_obj_set_flex_align(obj,
                                      LV_FLEX_ALIGN_SPACE_AROUND,
                                      LV_FLEX_ALIGN_CENTER,
                                      LV_FLEX_ALIGN_CENTER);
        }
        // **解析 flex_grow**
        if (const char *grow = xmlElem->Attribute("flex_grow")) {
            int growValue = std::stoi(grow);
            lv_obj_set_flex_grow(obj, growValue);
        }

        // **解析 flex_wrap**  换行被废弃了，使用grid代替
        // if (const char *wrap = xmlElem->Attribute("flex_wrap")) {
        //     if (strcmp(wrap, "wrap") == 0) lv_obj_set_flex_wrap(obj, LV_FLEX_WRAP_WRAP);
        //     else if (strcmp(wrap, "nowrap") == 0) lv_obj_set_flex_wrap(obj, LV_FLEX_WRAP_NOWRAP);
        // }
    }

    static void ApplyGridLayout(lv_obj_t *obj, XMLElement *xmlElem) {
        lv_obj_set_layout(obj, LV_LAYOUT_GRID);

        static lv_coord_t col_dsc[10];
        static lv_coord_t row_dsc[10];

        // **解析 grid 相关属性**
        if (const char *cols = xmlElem->Attribute("grid_cols")) {
            int num_cols = std::stoi(cols);
            for (int i = 0; i < num_cols; i++) col_dsc[i] = LV_GRID_FR(1);
            col_dsc[num_cols] = LV_GRID_TEMPLATE_LAST;
        }
        if (const char *rows = xmlElem->Attribute("grid_rows")) {
            int num_rows = std::stoi(rows);
            for (int i = 0; i < num_rows; i++) row_dsc[i] = LV_GRID_FR(1);
            row_dsc[num_rows] = LV_GRID_TEMPLATE_LAST;
        }

        lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);

        // **解析 col_index 和 row_index，将子项绑定到单元格**
        int col_index = xmlElem->IntAttribute("col_index", -1);
        int row_index = xmlElem->IntAttribute("row_index", -1);

        printf("col_index: %d, row_index: %d\n", col_index, row_index);

        if (col_index >= 0 && row_index >= 0) {
            lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, col_index, 1, LV_GRID_ALIGN_CENTER, row_index, 1);
        }
    }


    // **布局属性**
    static void ApplyLayoutAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        if (const char *layout = xmlElem->Attribute("layout")) {
            if (strcmp(layout, "grid") == 0) {
                ApplyGridLayout(obj, xmlElem);
            } else if (strcmp(layout, "flex") == 0) {
                ApplyFlexLayout(obj, xmlElem);
            }
        } else if (xmlElem->Attribute("grid")) {
            ApplyGridLayout(obj, xmlElem);
        }
    }


    // **主题属性**
    static void ApplyThemeAttributes(lv_obj_t *obj, const char *themeId) {
        auto it = themeStyles.find(themeId);
        if (it != themeStyles.end()) {
            for (const auto &[key, value]: it->second) {
                if (key == "background_color") {
                    lv_obj_set_style_bg_color(obj, lv_color_hex(std::stoul(value, nullptr, 16)), 0);
                } else if (key == "border_color") {
                    lv_obj_set_style_border_color(obj, lv_color_hex(std::stoul(value, nullptr, 16)), 0);
                }
            }
        }
    }
};

std::unordered_map<std::string, std::unordered_map<std::string, std::string> > StyleManager::themeStyles;


#endif //STYLE_MANAGER_H
