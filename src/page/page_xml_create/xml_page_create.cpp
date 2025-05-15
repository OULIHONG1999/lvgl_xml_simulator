//
// Created by Administrator on 2025/4/26.
//

#include "xml_page_create.h"

#include <iostream>
#include <unordered_map>

#include "../../page_manager/page_manager.h"
#include "image_loader/image_factory.h"
#include "src/lib/tinyxml/tinyxml2.h"


#define LV_DEFAULT_WIDTH 170
#define LV_DEFAULT_HEIGHT 320

using namespace tinyxml2;

// **工厂模式：组件基类**
class LVGLComponentFactory {
public:
    virtual ~LVGLComponentFactory() = default;

    virtual lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) = 0;

    virtual void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) = 0;
};

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

    // **布局属性**
    static void ApplyLayoutAttributes(lv_obj_t *obj, XMLElement *xmlElem) {
        if (const char *layout = xmlElem->Attribute("layout")) {
            if (strcmp(layout, "grid") == 0) {
                lv_obj_set_layout(obj, LV_LAYOUT_GRID);
            } else if (strcmp(layout, "flex") == 0) {
                lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
            }
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


// **屏幕工厂**
class ScreenFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *screen = lv_obj_create(parent);
        StyleManager::ApplyAttributes(screen, xmlElem);
        return screen;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};

// **按钮工厂**
class ButtonFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *btn = lv_btn_create(parent);
        StyleManager::ApplyAttributes(btn, xmlElem);
        return btn;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};

// **标签工厂**
class LabelFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *label = lv_label_create(parent);
        StyleManager::ApplyAttributes(label, xmlElem);
        return label;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};

// **图片工厂**
class ImgFactory : public LVGLComponentFactory {
public:
    static std::optional<std::string> GetImagePathFromXmlElement(XMLElement *xmlElem) {
        if (const char *src = xmlElem->Attribute("src"); src && std::strlen(src) > 0) {
            return std::string(src);
        }
        if (const char *path = xmlElem->Attribute("path"); path && std::strlen(path) > 0) {
            return std::string(path);
        }
        return std::nullopt;
    }

    // 添加不同lvgl图片的创建方法
    // 创建普通图片
    static lv_obj_t *CreateImage(lv_obj_t *parent, lv_img_dsc_t *lv_img_dsc) {
        lv_obj_t *image = lv_img_create(parent);
        lv_img_set_src(image, lv_img_dsc);
        return image;
    }

    // 创建gif
    static lv_obj_t *CreateGif(lv_obj_t *parent, lv_img_dsc_t *lv_img_dsc) {
        lv_obj_t *gif = lv_gif_create(parent);
        lv_gif_set_src(gif, lv_img_dsc);
        return gif;
    }

    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        auto imagePathOpt = GetImagePathFromXmlElement(xmlElem);
        if (!imagePathOpt) {
            std::cerr << "Image source not found!" << std::endl;
            return nullptr;
        }
        const std::string &imagePath = *imagePathOpt;
        static ImageResource res;
        // 尝试加载图片资源
        auto image = loader_image(imagePath, res);
        if (image) {
            // 处理加载的图像数据
            std::cout << "Image data: " << image->data.size() << " bytes" << std::endl;
        } else {
            std::cerr << "Failed to load image." << std::endl;
        }

        // 创建 LVGL 图像描述符
        static lv_img_dsc_t img_dsc;
        img_dsc.header.always_zero = 0;
        img_dsc.header.w = res.width;
        img_dsc.header.h = res.height;
        img_dsc.data = res.data.data(); // 确保数据格式匹配
        img_dsc.data_size = res.size; // 假设 size 是字节数
        img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA; // 根据实际颜色格式设置，如 LV_IMG_CF_TRUE_COLOR_ALPHA

        lv_obj_t *img = nullptr;
        // 判断图片类型
        if (res.type == ImageType::GIF) {
            img = CreateGif(parent, &img_dsc);
        } else if (res.type == ImageType::PNG) {
            img = CreateImage(parent, &img_dsc);
        } else if (res.type == ImageType::JPG) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED; // 根据实际颜色格式设置，如 LV_IMG_CF_TRUE_COLOR_ALPHA

            img = CreateImage(parent, &img_dsc);
        }

        if (img) {
            StyleManager::ApplyAttributes(img, xmlElem);
            return img;
        }

        std::cerr << "Unsupported image type!" << std::endl;
        return nullptr;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};

// **组件映射表**
std::unordered_map<std::string, LVGLComponentFactory *> componentFactories;


void RegisterComponentFactory(const std::string &tagName, LVGLComponentFactory *factory) {
    componentFactories[tagName] = factory;
}

// **递归解析 XML 并创建 LVGL 组件**
void ParseXML(lv_obj_t *parent, XMLElement *xmlElem) {
    if (!xmlElem) return;

    std::string tagName = xmlElem->Name();
    printf("Parsing tag: %s\n", tagName.c_str());

    auto factoryIt = componentFactories.find(tagName);
    if (factoryIt != componentFactories.end()) {
        lv_obj_t *obj = factoryIt->second->CreateComponent(parent, xmlElem);

        // 递归解析子组件
        XMLElement *child = xmlElem->FirstChildElement();
        if (!child) {
            printf("End of branch: %s has no children.\n", tagName.c_str());
        } else {
            while (child) {
                printf("Child tag: %s\n", child->Name());
                ParseXML(obj, child);
                child = child->NextSiblingElement();
            }
        }
    } else {
        printf("No factory found for tag: %s\n", tagName.c_str());
    }
}


// **解析 XML 生成界面**
void xml_page_create(const char *xml_path) {
    RegisterComponentFactory("screen", new ScreenFactory());
    RegisterComponentFactory("button", new ButtonFactory());
    RegisterComponentFactory("label", new LabelFactory());
    RegisterComponentFactory("img", new ImgFactory());

    XMLDocument doc;
    if (doc.LoadFile(xml_path) != XML_SUCCESS) {
        std::cerr << "XML 文件加载失败！" << std::endl;
        return;
    }

    XMLElement *root = doc.RootElement();
    if (!root) {
        std::cerr << "XML 解析失败，根节点为空！" << std::endl;
        return;
    }

    // **先加载主题**
    XMLElement *themeElem = root->FirstChildElement("theme");
    if (themeElem) {
        StyleManager::LoadTheme(themeElem);
    }

    // **确保 screen 解析正确**
    XMLElement *screenElem = root->FirstChildElement("screen");
    if (!screenElem) {
        std::cerr << "找不到 screen 元素，无法创建界面！" << std::endl;
        return;
    }

    lv_obj_t *screen = lv_scr_act();
    ParseXML(screen, screenElem);
}
