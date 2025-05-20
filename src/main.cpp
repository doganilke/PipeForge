#include <iostream>
#include <fstream>
#include <regex>

#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>

#include "downloadpipe.h"
#include "preprocessingpipe.h"
#include "dbscanpipe.h"
#include "pointdb.h"
#include "outputpipe.h"

using boost::optional;
using namespace boost::program_options;

// Checks if a file exists
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Attempts to read an image from disk; returns optional<cv::Mat>
optional<cv::Mat> readImage(const std::string& path) {
    if (!fileExists(path))
        return {};
    
    cv::Mat img = cv::imread(path);
    if (!img.data)
        return {};
    
    return img;
}

int main(int argc, const char* argv[]) {
    ImageInfo info;
    variables_map vm;

    // Define command line options
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print usage information")
        ("path", value<std::string>(), "Image file path or URL");

    try {
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
    }
    catch (const boost::exception& ex) {
        std::cerr << "Error parsing command line arguments. Use --help for usage.\n";
        return 1;
    }

    if (vm.count("help") || !vm.count("path")) {
        std::cout << desc << '\n';
        return 0;
    }

    const auto& inputPath = vm["path"].as<std::string>();

    // Simple URL regex to detect if input is URL
    const std::regex urlRegex(R"((https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?)", std::regex::icase);

    if (std::regex_match(inputPath, urlRegex)) {
        std::cout << "Processing input as URL...\n";
        info.url = inputPath;

        // Build pipeline for URL download + processing
        auto downloader = std::make_unique<UrlDownloadPipe>();
        auto preprocess = std::make_unique<PreprocessingPipe>();
        auto dbscan = std::make_unique<DbScanPipe>(MINIMUM_POINTS, EPSILON);
        auto output = std::make_unique<OutputPipe>();

        downloader->next(preprocess.get())->next(dbscan.get())->next(output.get());
        downloader->handle(info);
    }
    else {
        std::cout << "Processing input as file path...\n";
        auto imageOpt = readImage(inputPath);
        if (!imageOpt) {
            std::cerr << "Failed to read image from path: " << inputPath << '\n';
            return 1;
        }
        info.img = imageOpt.get();

        // Build pipeline for local image processing
        auto preprocess = std::make_unique<PreprocessingPipe>();
        auto dbscan = std::make_unique<DbScanPipe>(MINIMUM_POINTS, EPSILON);
        auto output = std::make_unique<OutputPipe>();

        preprocess->next(dbscan.get())->next(output.get());
        preprocess->handle(info);
    }

    return 0;
}
