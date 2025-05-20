#pragma once
#include <vector>
#include <cmath>
#include <memory>
#include "pipeline.h"
#include "nanoflann.hpp"
#include "pointcloud_adapter.h"

#define UNCLASSIFIED -1
#define NOISE -2
#define SUCCESS 0
#define FAILURE -3

class DbScanPipe : public Pipeline 
{
public:
    DbScanPipe(unsigned int minPts, float eps, std::vector<PointDb> points = std::vector<PointDb>())
        : mMinPoints(minPts), mEpsilon(eps), mPoints(points), mPointSize(points.size()) {}

    virtual ~DbScanPipe() {}

    void handle(ImageInfo&) override;

private:
    void run();
    std::vector<int> calculateCluster(const PointDb& point);
    int expandCluster(int pointIndex, int clusterID);
    inline double calculateDistance(const PointDb& a, const PointDb& b) const;

    std::vector<PointDb> mPoints;
    unsigned int mPointSize;
    unsigned int mMinPoints;
    float mEpsilon;

    struct PointCloudAdaptorWrapper {
        std::vector<PointDb>* pts;

        inline size_t kdtree_get_point_count() const { return pts->size(); }

        inline float kdtree_distance(const float* p1, const size_t idx_p2, size_t) const {
            const PointDb& p2 = pts->at(idx_p2);
            float dx = p1[0] - p2.x;
            float dy = p1[1] - p2.y;
            float dz = p1[2] - p2.z;
            return dx * dx + dy * dy + dz * dz;
        }

        inline float kdtree_get_pt(const size_t idx, int dim) const {
            const PointDb& p = pts->at(idx);
            if (dim == 0) return p.x;
            else if (dim == 1) return p.y;
            else return p.z;
        }

        template <class BBOX>
        bool kdtree_get_bbox(BBOX&) const { return false; }
    };

    typedef nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Simple_Adaptor<float, PointCloudAdaptorWrapper>,
        PointCloudAdaptorWrapper,
        3
    > KDTree;

    std::unique_ptr<KDTree> kdtree;
    PointCloudAdaptorWrapper pcAdaptor;
};
