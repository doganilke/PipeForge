#pragma once

#define MINIMUM_POINTS 1     // minimum number of cluster
#define EPSILON (50)  // distance for clustering, metre^2

struct PointDb
{
	float x, y, z;  // X, Y, Z position

	int clusterID;  // clustered ID

	bool operator()(const PointDb& a, const PointDb& b) const { return a.x == b.x && a.y == b.y && a.z == b.z && a.clusterID == b.clusterID; }
};

struct ImageInfo
{
	ImageInfo(){}

	cv::Mat img;

	std::vector<std::vector<cv::Point> >contours;

	std::vector<cv::Point2f>mc;

	std::vector<PointDb> points;

	std::string url;
	
};