#pragma once

#include <curl/curl.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "pipeline.h"

class IDownloader {
public:
    virtual ~IDownloader() = default;
    virtual bool download(ImageInfo& info) = 0;
};

// Downloads an image from a URL using libcurl
class UrlDownloadPipe : public IDownloader, public Pipeline {
public:
    UrlDownloadPipe() = default;
    ~UrlDownloadPipe() override = default;

    bool download(ImageInfo& info) override;
    void handle(ImageInfo& info) override;

private:
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
};
