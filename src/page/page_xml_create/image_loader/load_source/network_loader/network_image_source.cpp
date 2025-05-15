//
// Created by Administrator on 2025/5/15.
//

#include "network_image_source.h"
#include "../../image_source.h"

class NetworkImageSource : public ImageSource {
public:
    bool LoadImage(const std::string& url, ImageResource& resource) override {
        // 这里需要实现网络请求，例如 HTTP 下载逻辑
        resource.sourceType = "Network";

        return true;
    }
};
