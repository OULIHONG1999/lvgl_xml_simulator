//
// Created by Administrator on 2025/8/29.
//

#ifndef NETWORKIMAGELOADER_H
#define NETWORKIMAGELOADER_H
#include <string>
#include <vector>
#define _URLMON_NO_MSXML

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "winhttp.lib")

class NetworkImageLoader {
public:
    static ImageType DetectImageType(const std::vector<unsigned char> &data);

    static bool LoadFromUrl(const std::string &url, ImageResource &res);
};

inline ImageType NetworkImageLoader::DetectImageType(const std::vector<unsigned char> &data) {
    if (data.size() >= 8 &&
        data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47)
        return ImageType::PNG;
    if (data.size() >= 3 &&
        data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF)
        return ImageType::JPEG;
    if (data.size() >= 2 &&
        data[0] == 'B' && data[1] == 'M')
        return ImageType::BMP;
    if (data.size() >= 3 &&
        data[0] == 'G' && data[1] == 'I' && data[2] == 'F')
        return ImageType::GIF;
    return ImageType::UNKNOWN;
}

inline bool NetworkImageLoader::LoadFromUrl(const std::string &url, ImageResource &res) {
    res.release();

    HINTERNET hInternet = InternetOpenA("NetworkImageLoader/1.0",
                                        INTERNET_OPEN_TYPE_PRECONFIG,
                                        NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(),
                                       NULL, 0,
                                       INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
                                       0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[4096];
    DWORD bytesRead = 0;

    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        res.data.insert(res.data.end(), buffer, buffer + bytesRead);
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    if (!res.data.empty()) {
        res.size = static_cast<unsigned int>(res.data.size());
        res.sourceType = "network";
        res.type = DetectImageType(res.data);
        return true;
    }
    return false;
}

#endif //NETWORKIMAGELOADER_H
