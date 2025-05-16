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
        SetObjAttributes(btn, xmlElem);
        return btn;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        // 查找是否有文本标签
        if (const char *text = xmlElem->Attribute("text"); text && std::strlen(text) > 0) {
            lv_obj_t *label = lv_label_create(obj);
            lv_label_set_text(label, text);
            // 居中
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            // 设置默认字体
            lv_obj_set_style_text_font(label, findFontBySize(10), 0);
        }
    }
};

// **标签工厂**
class LabelFactory : public LVGLComponentFactory {
public:
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        lv_obj_t *label = lv_label_create(parent);
        StyleManager::ApplyAttributes(label, xmlElem);
        SetObjAttributes(label, xmlElem);
        return label;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
        // 设置默认字体
        lv_obj_set_style_text_font(obj, findFontBySize(10), 0);
        // 设置标签的文本
        if (const char *text = xmlElem->Attribute("text"); text && std::strlen(text) > 0) {
            lv_label_set_text(obj, text);
        } else
            lv_label_set_text(obj, "");
    }
};


// **图片工厂**
class ImgFactory : public LVGLComponentFactory {
public:
    struct ManagedImage {
        lv_img_dsc_t img_dsc{};
        ImageResource imgRes;
    };

    using ImageHandle = std::shared_ptr<ManagedImage>;

    // 资源池（供后续统一管理）
    static std::vector<ImageHandle> imagePool;

    // 动态创建并返回 img_dsc 的智能指针
    static ImageHandle CreateAndManageImage(const std::string &imagePath) {
        ImageResource res;
        auto image = loader_image(imagePath, res);
        if (!image) {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            return nullptr;
        }

        ImageHandle handle = std::make_shared<ManagedImage>();
        handle->imgRes = std::move(res);

        // 初始化 img_dsc
        auto &img_dsc = handle->img_dsc;
        memset(&img_dsc, 0, sizeof(lv_img_dsc_t));
        img_dsc.header.always_zero = 0;
        img_dsc.header.w = handle->imgRes.width;
        img_dsc.header.h = handle->imgRes.height;
        img_dsc.data = handle->imgRes.data.data();
        img_dsc.data_size = handle->imgRes.size;

        // 根据类型设置 color format
        if (handle->imgRes.type == ImageType::PNG) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
        } else if (handle->imgRes.type == ImageType::JPG) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED;
        } else if (handle->imgRes.type == ImageType::GIF) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
        } else {
            std::cerr << "Unsupported image type" << std::endl;
            return nullptr;
        }

        // 加入资源池
        imagePool.push_back(handle);
        return handle;
    }

    // 手动释放所有图像资源
    static void ClearAllImages() {
        imagePool.clear(); // shared_ptr 自动析构
    }

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
        ImageHandle handle = CreateAndManageImage(imagePath);


        lv_obj_t *img = nullptr;

        if (handle) {
            // 判断图片类型
            switch (handle->imgRes.type) {
                case ImageType::GIF: {
                    img = CreateGif(parent, &handle->img_dsc);
                }
                break;
                case ImageType::JPG:
                case ImageType::PNG: {
                    img = CreateImage(parent, &handle->img_dsc);
                }
                break;
                default: {
                    std::cerr << "Unsupported image type!" << std::endl;
                    // 尝试释放资源
                    if (handle) {
                        handle.reset();
                    }
                    return nullptr;
                }
            }

            if (img) {
                StyleManager::ApplyAttributes(img, xmlElem);
                return img;
            }
        }

        std::cerr << "Unsupported image type!" << std::endl;
        return nullptr;
    }

    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};
// 定义 ImgFactory 的静态成员变量
std::vector<ImgFactory::ImageHandle> ImgFactory::imagePool;
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
