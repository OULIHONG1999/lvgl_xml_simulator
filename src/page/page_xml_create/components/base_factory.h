//
// Created by Administrator on 2025/6/10.
//

#ifndef BASE_FACTORY_H
#define BASE_FACTORY_H

#include <unordered_map>
#include <string>

#include "../../page_manager/page_manager.h"
#include "src/lib/tinyxml/tinyxml2.h"

#include "../../pika_python/pikaPython.h"

using namespace tinyxml2;


using namespace tinyxml2;

class LVGLComponentFactory {
public:
    virtual ~LVGLComponentFactory() = default;

    virtual lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) = 0;

    virtual void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
    }

    virtual void SetCommonAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        SetSizeAttributes(obj, xmlElem);
        SetLayoutAttributes(obj, xmlElem);
        SetStyleAttributes(obj, xmlElem);
        SetStateAttributes(obj, xmlElem);
        SetInteractionAttributes(obj, xmlElem);
        SetAnimationAttributes(obj, xmlElem);
        SetEventAttributes(obj, xmlElem);
        SetBindingAttributes(obj, xmlElem);
    }

protected:
    static void SetSizeAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *w = xml->Attribute("width")) lv_obj_set_width(obj, ParseSize(w));
        if (const char *h = xml->Attribute("height")) lv_obj_set_height(obj, ParseSize(h));
        if (const char *align = xml->Attribute("align")) lv_obj_align(obj, ParseAlign(align), 0, 0);
    }

    static void SetLayoutAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *pad = xml->Attribute("pad")) {
            int val = SafeParseInt(pad, 0);
            lv_obj_set_style_pad_all(obj, val, LV_PART_MAIN);
        }
        if (const char *rowGap = xml->Attribute("row_gap")) {
            lv_obj_set_style_pad_row(obj, SafeParseInt(rowGap, 0), LV_PART_MAIN);
        }
        if (const char *colGap = xml->Attribute("column_gap")) {
            lv_obj_set_style_pad_column(obj, SafeParseInt(colGap, 0), LV_PART_MAIN);
        }
        if (const char *flow = xml->Attribute("flex_flow")) {
            lv_obj_set_flex_flow(obj, ParseFlexFlow(flow));
        }
    }

    static void SetStyleAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *bg = xml->Attribute("bg_color")) {
            lv_color_t c = lv_color_hex(strtol(bg + 1, nullptr, 16));
            lv_obj_set_style_bg_color(obj, c, LV_PART_MAIN);
        }
        if (const char *opa = xml->Attribute("bg_opa")) {
            lv_obj_set_style_bg_opa(obj, SafeParseInt(opa, 255), LV_PART_MAIN);
        }
        if (const char *radius = xml->Attribute("radius")) {
            lv_obj_set_style_radius(obj, SafeParseInt(radius, 0), LV_PART_MAIN);
        }
        if (const char *shadow = xml->Attribute("shadow_width")) {
            lv_obj_set_style_shadow_width(obj, SafeParseInt(shadow, 0), LV_PART_MAIN);
        }
    }

    static void SetStateAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (IsTrue(xml->Attribute("hidden"))) lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        if (IsTrue(xml->Attribute("disabled"))) lv_obj_add_state(obj, LV_STATE_DISABLED);
        if (IsTrue(xml->Attribute("checked"))) lv_obj_add_state(obj, LV_STATE_CHECKED);
    }

    static void SetInteractionAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (IsTrue(xml->Attribute("clickable"))) lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        if (IsTrue(xml->Attribute("draggable"))) lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }

    static void SetAnimationAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *anim = xml->Attribute("anim")) {
            // 你可以在此调用自定义动画注册器
            printf("animation: %s\n", anim);
        }
    }

    static void SetEventAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *evt = xml->Attribute("event")) {
            if (strcmp(evt, "on_click") == 0) {
                lv_obj_add_event_cb(obj, [](lv_event_t *e) {
                    printf("clisk event\n");
                }, LV_EVENT_CLICKED, nullptr);
            }
        }
    }

    static void SetBindingAttributes(lv_obj_t *obj, const XMLElement *xml) {
        if (const char *subject = xml->Attribute("subject")) {
            printf("data  subject: %s\n", subject);
            // 你可以在此绑定 subject 到对象
        }
    }

    static int ParseSize(const char *val) {
        if (!val) return 0;
        if (strcmp(val, "100%") == 0) return LV_SIZE_CONTENT;
        errno = 0;
        char *endptr = nullptr;
        long result = std::strtol(val, &endptr, 10);
        if (errno || result < 0 || result > INT_MAX || endptr == val || *endptr != '\0') return 0;
        return static_cast<int>(result);
    }

    static lv_align_t ParseAlign(const char *val) {
        if (!val) return LV_ALIGN_DEFAULT;
        if (strcmp(val, "center") == 0) return LV_ALIGN_CENTER;
        if (strcmp(val, "top") == 0) return LV_ALIGN_TOP_MID;
        if (strcmp(val, "bottom") == 0) return LV_ALIGN_BOTTOM_MID;
        if (strcmp(val, "left") == 0) return LV_ALIGN_LEFT_MID;
        if (strcmp(val, "right") == 0) return LV_ALIGN_RIGHT_MID;
        if (strcmp(val, "top_left") == 0) return LV_ALIGN_TOP_LEFT;
        if (strcmp(val, "top_right") == 0) return LV_ALIGN_TOP_RIGHT;
        if (strcmp(val, "bottom_left") == 0) return LV_ALIGN_BOTTOM_LEFT;
        if (strcmp(val, "bottom_right") == 0) return LV_ALIGN_BOTTOM_RIGHT;
        return LV_ALIGN_DEFAULT;
    }

    static lv_flex_flow_t ParseFlexFlow(const char *val) {
        if (!val) return LV_FLEX_FLOW_ROW;
        if (strcmp(val, "row") == 0) return LV_FLEX_FLOW_ROW;
        if (strcmp(val, "row_wrap") == 0) return LV_FLEX_FLOW_ROW_WRAP;
        if (strcmp(val, "row_reverse") == 0) return LV_FLEX_FLOW_ROW_REVERSE;
        if (strcmp(val, "column") == 0) return LV_FLEX_FLOW_COLUMN;
        if (strcmp(val, "column_wrap") == 0) return LV_FLEX_FLOW_COLUMN_WRAP;
        if (strcmp(val, "column_reverse") == 0) return LV_FLEX_FLOW_COLUMN_REVERSE;
        return LV_FLEX_FLOW_ROW;
    }

    static int SafeParseInt(const char *str, int defaultValue = 0) {
        if (!str) return defaultValue;
        errno = 0;
        char *endptr = nullptr;
        long val = std::strtol(str, &endptr, 10);
        if (errno || endptr == str || *endptr != '\0' || val < INT_MIN || val > INT_MAX) return defaultValue;
        return static_cast<int>(val);
    }

    static bool IsTrue(const char *val) {
        return val && strcmp(val, "true") == 0;
    }
};


#endif //BASE_FACTORY_H
