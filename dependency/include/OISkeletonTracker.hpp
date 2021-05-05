#ifndef OPENISS_SKELETON_TRACKER_H
#define OPENISS_SKELETON_TRACKER_H

#include "OITracker.hpp"

namespace openiss
{

/**
 * The main goal of tracker is to get skeleton:
 *    Tracker -> TrackerFrame -> UserData -> Skeleton
 */
class OISkeletonTracker : public OITracker
{
public:
    virtual ~OISkeletonTracker() = default;

    virtual void mapJoint2Depth(float x, float y, float z, float* pOutX, float* pOutY) const = 0;
    virtual void mapDepth2Joint(int x, int y, int z, float* pOutX, float* pOutY) const = 0;
};

} // namespace

#endif // OPENISS_SKELETON_TRACKER_H

// EOF
