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


// **图片工厂类**
// 继承自 LVGLComponentFactory，负责：
// 1. 从 XML 元素读取图片路径
// 2. 调用底层 loader 加载图片数据
// 3. 根据图片类型（PNG/JPG/GIF）创建对应的 LVGL 控件
// 4. 管理图片资源生命周期（通过资源池统一管理，避免内存泄漏）
class ImgFactory : public LVGLComponentFactory {
public:

    // 封装已加载图片的结构
    // img_dsc  是 LVGL 显示描述（尺寸、颜色格式、数据指针等）
    // imgRes   是原始图片数据对象
    struct ManagedImage {
        lv_img_dsc_t img_dsc{};
        ImageResource imgRes;
    };

    // 统一用 shared_ptr 来管理图片资源，避免手动 delete
    using ImageHandle = std::shared_ptr<ManagedImage>;

    // 静态资源池，保存当前所有已创建的图片句柄
    // 方便全局释放或批量管理
    static std::vector<ImageHandle> imagePool;

    /**
     * @brief 加载并管理一张图片
     * @param imagePath  图片路径
     * @return  图片句柄（智能指针）
     *
     * 逻辑：
     * 1. 调用 loader_image 加载到 ImageResource
     * 2. 创建 ManagedImage 对象
     * 3. 初始化 lv_img_dsc_t 的头信息（尺寸、颜色格式等）
     * 4. 推入 imagePool，供后续管理
     */
    static ImageHandle CreateAndManageImage(const std::string &imagePath) {
        ImageResource res;
        auto image = loader_image(imagePath, res);
        if (!image) {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            return nullptr;
        }

        ImageHandle handle = std::make_shared<ManagedImage>();
        handle->imgRes = std::move(res);

        // 初始化 LVGL 图片描述结构
        auto &img_dsc = handle->img_dsc;
        memset(&img_dsc, 0, sizeof(lv_img_dsc_t));
        img_dsc.header.always_zero = 0;
        img_dsc.header.w = handle->imgRes.width;
        img_dsc.header.h = handle->imgRes.height;
        img_dsc.data = handle->imgRes.data.data();
        img_dsc.data_size = handle->imgRes.size;

        // 根据不同类型设置颜色格式
        if (handle->imgRes.type == ImageType::PNG) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA; // 透明通道
        } else if (handle->imgRes.type == ImageType::JPG) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED; // 透明色键
        } else if (handle->imgRes.type == ImageType::GIF) {
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
        } else {
            std::cerr << "Unsupported image type" << std::endl;
            return nullptr;
        }

        // 放入静态资源池
        imagePool.push_back(handle);
        return handle;
    }

    /**
     * @brief 清空所有图片资源
     * shared_ptr 会自动析构释放实际内存
     */
    static void ClearAllImages() {
        imagePool.clear();
    }

    /**
     * @brief 从 XML 元素获取图片路径
     * 支持 "src" 或 "path" 属性
     */
    static std::optional<std::string> GetImagePathFromXmlElement(XMLElement *xmlElem) {
        if (const char *src = xmlElem->Attribute("src"); src && std::strlen(src) > 0) {
            return std::string(src);
        }
        if (const char *path = xmlElem->Attribute("path"); path && std::strlen(path) > 0) {
            return std::string(path);
        }
        return std::nullopt;
    }

    /**
     * @brief 创建普通静态图片
     * 使用 lv_img_create
     */
    static lv_obj_t *CreateImage(lv_obj_t *parent, lv_img_dsc_t *lv_img_dsc) {
        lv_obj_t *image = lv_img_create(parent);
        lv_img_set_src(image, lv_img_dsc);
        return image;
    }

    /**
     * @brief 创建 GIF 图片
     * 使用 lv_gif_create
     */
    static lv_obj_t *CreateGif(lv_obj_t *parent, lv_img_dsc_t *lv_img_dsc) {
        lv_obj_t *gif = lv_gif_create(parent);
        lv_gif_set_src(gif, lv_img_dsc);
        return gif;
    }

    /**
     * @brief 工厂主入口，从 XML 元素创建图片控件
     * 1. 获取路径
     * 2. 加载图片
     * 3. 根据类型创建普通图片或 GIF
     * 4. 应用通用属性 + 对象属性
     */
    lv_obj_t *CreateComponent(lv_obj_t *parent, XMLElement *xmlElem) override {
        auto imagePathOpt = GetImagePathFromXmlElement(xmlElem);
        if (!imagePathOpt) {
            std::cerr << "Image source not found!" << std::endl;
            return nullptr;
        }
        const std::string &imagePath = *imagePathOpt;
        static ImageResource res;

        // 尝试预加载并输出调试信息
        auto image = loader_image(imagePath, res);
        // auto image = loader_image_auto(imagePath, res);
        if (image) {
            std::cout << "Image data: " << image->data.size() << " bytes" << std::endl;
        } else {
            std::cerr << "Failed to load image." << std::endl;
        }

        // 创建并管理图片资源
        ImageHandle handle = CreateAndManageImage(imagePath);
        lv_obj_t *img = nullptr;

        if (handle) {
            // 根据图片类型选择控件
            switch (handle->imgRes.type) {
                case ImageType::GIF:
                    img = CreateGif(parent, &handle->img_dsc);
                    break;
                case ImageType::JPG:
                case ImageType::PNG:
                    img = CreateImage(parent, &handle->img_dsc);
                    break;
                default:
                    std::cerr << "Unsupported image type!" << std::endl;
                    handle.reset();
                    return nullptr;
            }

            // 设置 LVGL 公共属性 & 专有属性
            if (img) {
                SetCommonAttributes(img, xmlElem); // 通用属性（x/y/size/style等）
                SetObjAttributes(img, xmlElem);    // 特定控件属性（可扩展）
                return img;
            }
        }

        std::cerr << "Unsupported image type!" << std::endl;
        return nullptr;
    }

    /**
     * @brief 针对图片控件的专有属性设置
     * 当前为空，可以扩展：
     * - 旋转角度
     * - 缩放比例
     * - 对齐方式
     */
    void SetObjAttributes(lv_obj_t *obj, XMLElement *xmlElem) override {
    }
};

// 静态成员初始化
std::vector<ImgFactory::ImageHandle> ImgFactory::imagePool;

#endif //IMG_FACTORY_H
