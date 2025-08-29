//
// Created by Administrator on 2025/8/29.
//

#ifndef NETWORKIMAGELOADER_H
#define NETWORKIMAGELOADER_H


class NetworkImageLoader {
public:
    static bool LoadFromUrl(const std::string &path, const ImageResource &res);
};


inline bool NetworkImageLoader::LoadFromUrl(const std::string &path, const ImageResource &res) {
    return false;
}
#endif //NETWORKIMAGELOADER_H
