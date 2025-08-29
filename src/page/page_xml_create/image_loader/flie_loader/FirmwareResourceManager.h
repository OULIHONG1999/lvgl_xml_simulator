//
// Created by Administrator on 2025/8/29.
//

#ifndef FIRMWARERESOURCEMANAGER_H
#define FIRMWARERESOURCEMANAGER_H



class FirmwareResourceManager {
public:
    static bool Exists(const std::string & path);

    static bool LoadImage(const std::string & path, const ImageResource & res);
};


inline bool FirmwareResourceManager::Exists(const std::string &path) {
    return false;
}

inline bool FirmwareResourceManager::LoadImage(const std::string &path, const ImageResource &res) {
    return false;
}
#endif //FIRMWARERESOURCEMANAGER_H
