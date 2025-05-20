#include "dbscanpipe.h"

void DbScanPipe::handle(ImageInfo& info) {
    std::vector<PointDb> points;

    for (const auto& r : info.mc) {
        PointDb p;
        p.x = r.x;
        p.y = r.y;
        p.z = 0.0;
        p.clusterID = UNCLASSIFIED;
        points.push_back(p);
    }

    mPoints = std::move(points);
    mPointSize = mPoints.size();
    run();
    info.points = mPoints;
    Pipeline::handle(info);
}

void DbScanPipe::run() {
    pcAdaptor.pts = &mPoints;
    kdtree = std::make_unique<KDTree>(3, pcAdaptor, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    kdtree->buildIndex();

    int clusterID = 1;
    for (size_t i = 0; i < mPoints.size(); ++i) {
        if (mPoints[i].clusterID == UNCLASSIFIED) {
            if (expandCluster(static_cast<int>(i), clusterID) != FAILURE) {
                ++clusterID;
            }
        }
    }
}

int DbScanPipe::expandCluster(int pointIndex, int clusterID) {
    std::vector<int> seeds = calculateCluster(mPoints[pointIndex]);

    if (seeds.size() < mMinPoints) {
        mPoints[pointIndex].clusterID = NOISE;
        return FAILURE;
    }

    for (int idx : seeds) {
        mPoints[idx].clusterID = clusterID;
    }

    size_t i = 0;
    while (i < seeds.size()) {
        std::vector<int> neighbors = calculateCluster(mPoints[seeds[i]]);
        if (neighbors.size() >= mMinPoints) {
            for (int nIdx : neighbors) {
                if (mPoints[nIdx].clusterID == UNCLASSIFIED || mPoints[nIdx].clusterID == NOISE) {
                    if (mPoints[nIdx].clusterID == UNCLASSIFIED) {
                        seeds.push_back(nIdx);
                    }
                    mPoints[nIdx].clusterID = clusterID;
                }
            }
        }
        ++i;
    }

    return SUCCESS;
}

std::vector<int> DbScanPipe::calculateCluster(const PointDb& point) {
    std::vector<nanoflann::ResultItem<unsigned int, float>> ret_matches;
    nanoflann::SearchParameters params;
    float query_pt[3] = { point.x, point.y, point.z };
    float radius = mEpsilon * mEpsilon;

    kdtree->radiusSearch(&query_pt[0], radius, ret_matches, params);

    std::vector<int> indices;
    for (const auto& match : ret_matches) {
        indices.push_back(static_cast<int>(match.first)); 
    }

    return indices;
}

inline double DbScanPipe::calculateDistance(const PointDb& a, const PointDb& b) const {
    return std::sqrt(
        (a.x - b.x) * (a.x - b.x) +
        (a.y - b.y) * (a.y - b.y) +
        (a.z - b.z) * (a.z - b.z)
    );
}
