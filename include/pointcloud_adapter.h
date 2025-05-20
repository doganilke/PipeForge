#pragma once

#include <vector>
#include "pointdb.h"

// This adapter lets nanoflann use std::vector<PointDb> as a KD-Tree-compatible point cloud
struct PointCloudAdaptorWrapper {
    std::vector<PointDb>* pts;

    // Total number of points in the cloud
    inline size_t kdtree_get_point_count() const {
        return pts->size();
    }

    // Compute the squared distance between the query point and point at index idx_p2
    inline float kdtree_distance(const float* p1, const size_t idx_p2, size_t /*dim*/) const {
        const PointDb& p2 = pts->at(idx_p2);
        float dx = p1[0] - p2.x;
        float dy = p1[1] - p2.y;
        float dz = p1[2] - p2.z;
        return dx * dx + dy * dy + dz * dz;
    }

    // Return the requested coordinate (0=x, 1=y, 2=z) of the idx-th point
    inline float kdtree_get_pt(const size_t idx, int dim) const {
        const PointDb& p = pts->at(idx);
        if (dim == 0) return p.x;
        else if (dim == 1) return p.y;
        else return p.z;
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const {
        return false;
    }
};
