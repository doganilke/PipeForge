#include "downloadpipe.h"

bool UrlDownloadPipe::download(ImageInfo& info) {
    std::vector<uchar> buffer;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL\n";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, info.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        return false;
    }

    if (buffer.empty()) {
        std::cerr << "Downloaded data is empty\n";
        return false;
    }

    info.img = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);
    return !info.img.empty();
}

size_t UrlDownloadPipe::writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t totalSize = size * nmemb;
    auto* buffer = static_cast<std::vector<uchar>*>(userdata);
    buffer->insert(buffer->end(), ptr, ptr + totalSize);
    return totalSize;
}

void UrlDownloadPipe::handle(ImageInfo& info) {
    if (download(info)) {
        Pipeline::handle(info);  // Only continue pipeline if download succeeds
    } else {
        std::cerr << "Image download failed for URL: " << info.url << "\n";
    }
}
