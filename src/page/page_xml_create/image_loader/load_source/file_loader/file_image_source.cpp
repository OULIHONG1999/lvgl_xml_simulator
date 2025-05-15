//
// Created by Administrator on 2025/5/15.
//

#include "file_image_source.h"
#include <fstream>

#include "../../image_source.h"


class FileImageSource : public ImageSource {
public:
    bool LoadImage(const std::string &path, ImageResource &resource) override {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) return false;

        resource.size = file.tellg();
        file.seekg(0, std::ios::beg);
        resource.data.resize(resource.size);

        if (!file.read(reinterpret_cast<char *>(resource.data.data()), resource.size)) return false;

        resource.sourceType = "File";

        if (resource.size >= 10) {
            resource.width = resource.data[6] | (resource.data[7] << 8);
            resource.height = resource.data[8] | (resource.data[9] << 8);
        }

        return true;
    }
};
