#include "outputpipe.h"

void OutputPipe::handle(ImageInfo& info)
{
	printResults(info.points, info.points.size());
	writeImage(info);
	Pipeline::handle(info);
}

void OutputPipe::printResults(std::vector<PointDb>& points, int numPoints)
{
	int i = 0;
	printf("Number of points: %u\n"
		"     x         y         z       cluster_id\n"
		"---------------------------------------------\n"
		, numPoints);
	while (i < numPoints)
	{
		printf("%8.2lf %8.2lf %8.2lf: %5d\n",
			points[i].x,
			points[i].y,
			points[i].z,
			points[i].clusterID);
		++i;
	}
	std::set<int>clusters;
	for (auto r : points)
	{
		clusters.insert(r.clusterID);
	}
	std::cout << "Row count is : " << clusters.size() << "\n";
}

void OutputPipe::writeImage(ImageInfo& info)
{
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat drawing(info.img.size(), CV_8UC3, cv::Scalar(255, 255, 255));

    for (int i = 0; i < info.contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(167, 151, 0); // B G R values
        cv::drawContours(drawing, info.contours, i, color, 2, 8, hierarchy, 0, cv::Point());
    }

    int i = 3;
    for (const auto& r : info.points)
    {
        cv::circle(drawing, cv::Point(r.x, r.y), 50, cv::Scalar(255, (i * 6) % 255, (i * 9) % 255), cv::FILLED, 8, 0);
        i += 7;
    }

    cv::imwrite("contours.jpg", info.img);
    cv::imwrite("centerImage.jpg", drawing);
}
