#include "preprocessingpipe.h"
#include <iostream>
#include <opencv2/imgproc.hpp> 
#include <opencv2/core.hpp> 
#include <vector>
void PreprocessingPipe::handle(ImageInfo& info) {
    preprocessImage(info);
    Pipeline::handle(info);
}

void PreprocessingPipe::preprocessImage(ImageInfo& info) {
    try {
        // Work on a copy of the original image to keep info.img intact until processing done
        cv::Mat gray, processed;
        
        // Convert to grayscale
        cv::cvtColor(info.img, gray, cv::COLOR_BGR2GRAY);

        // Apply Canny edge detection (better to do on blurred image, so blur first)
        cv::blur(gray, gray, cv::Size(3, 3));
        cv::Canny(gray, processed, 120, 200, 3);

        // Morphological operations
        const cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
        const cv::Mat erosionKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

        cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, morphKernel, cv::Point(-1, -1), iterations);
        cv::morphologyEx(processed, processed, cv::MORPH_ERODE, erosionKernel, cv::Point(-1, -1), iterations + 6);

        info.img = processed;  // Replace original img with processed edges

        // Find centers of contours in processed image
        info.mc = findCenter(processed, info);
    }
    catch (const std::exception& e) {
        std::cerr << "Preprocessing error: " << e.what() << std::endl;
    }
}

std::vector<cv::Point2f> PreprocessingPipe::findCenter(const cv::Mat& img, ImageInfo& info) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point2f> centers;
    centers.reserve(contours.size());

    info.contours = std::move(contours); // Move contours to info

    for (const auto& contour : info.contours) {
        cv::Moments mu = cv::moments(contour);

        // Avoid division by zero
        if (mu.m00 != 0) {
            centers.emplace_back(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
        }
        else {
            centers.emplace_back(-1.f, -1.f); // invalid centroid (or skip pushing)
        }
    }

    return centers;
}
