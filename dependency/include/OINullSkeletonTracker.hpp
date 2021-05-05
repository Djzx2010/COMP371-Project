#ifndef OPENISS_NULL_SKELETON_TRACKER_H
#define OPENISS_NULL_SKELETON_TRACKER_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "OISkeletonTracker.hpp"

namespace openiss
{

/**
 * Null skeleton tracker for developlment and testing.
 * Provides stub skeleton data in the proper format.
 * @author Serguei Mokhov
 */
class OINullSkeletonTracker : public OISkeletonTracker
{
public:
    explicit OINullSkeletonTracker(OIDevice& pDev);
    ~OINullSkeletonTracker();

    OIStatus init() override;
    OIStatus update() override;
    OIStatus stop() override;

    void startTracking() override;
    void stopTracking() override;

    OITrackerFrame* readFrame(OITrackerFrame*) override;

    void mapJoint2Depth(float x, float y, float z, float* pOutX, float* pOutY) const override;
    void mapDepth2Joint(int x, int y, int z, float* pOutX, float* pOutY) const override;

private:
    // disable copy constructor and copy assignment
    OINullSkeletonTracker(const OINullSkeletonTracker&);
    OINullSkeletonTracker& operator=(OINullSkeletonTracker&);
};

class OINullTrackerFrame : public OITrackerFrame 
{
public:
    OINullTrackerFrame();
    ~OINullTrackerFrame();

    const std::vector<std::shared_ptr<OIUserData>> getUsers() override;
    const OIUserData& getUserById(int userId) const override;
    //const OIUserMap &getUserMap() const override;
    //std::vector<JointType> &getSupportedJointType() override;
    //void update(nite::UserTracker *tracker, nite::UserTrackerFrameRef &frameRef);
    void update();

protected:

    OINullTrackerFrame(const OINullTrackerFrame& frame);
    OINullTrackerFrame& operator = (const OINullTrackerFrame& frame);
};

} // end of namespace

#endif // OPENISS_NULL_SKELETON_TRACKER_H
