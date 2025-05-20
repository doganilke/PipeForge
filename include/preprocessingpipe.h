#pragma once
#include "pipeline.h"

class PreprocessingPipe : public Pipeline {
public:
    void handle(ImageInfo&) override;
    virtual ~PreprocessingPipe() = default;

private:
    void preprocessImage(ImageInfo& info);
    std::vector<cv::Point2f> findCenter(const cv::Mat& img, ImageInfo& info);

    const int iterations = 3;
};
