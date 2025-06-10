//
// Created by Administrator on 2025/6/10.
//

#ifndef IMG_FACTORY_H
#define IMG_FACTORY_H
#include <iostream>
#include <memory>

#include "base_factory.h"
#include "src/page/page_xml_create/image_loader/image_factory.h"
#include "src/page/page_xml_create/image_loader/image_resource.h"
#include "src/page/page_xml_create/style/style_manager.h"


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


#endif //IMG_FACTORY_H
