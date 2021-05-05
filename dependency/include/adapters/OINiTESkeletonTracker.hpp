//
// Created by Haotao Lai on 2018-08-01.
//

#ifndef OPENISS_NITE_SKELETON_TRACKER_H
#define OPENISS_NITE_SKELETON_TRACKER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <NiTE.h>
#include "OISkeletonTracker.hpp"

using std::shared_ptr;

namespace openiss
{

class OINiTESkeletonTracker : public OISkeletonTracker
{
public:
    explicit OINiTESkeletonTracker(OIDevice& pDev);
    ~OINiTESkeletonTracker();

    void startTracking() override;
    void stopTracking() override;
    
    OITrackerFrame* readFrame(OITrackerFrame* p_poTrackerFrame = nullptr) override;

    void mapJoint2Depth(float x, float y, float z, float *pOutX, float *pOutY) const override;
    void mapDepth2Joint(int x, int y, int z, float *pOutX, float *pOutY) const override;

private:
    nite::UserTracker* mpTracker;
    nite::UserTrackerFrameRef mNiteTrackerFrame;
    nite::Status mStatus;

    // disable copy constructor and copy assignment
    OINiTESkeletonTracker(const OINiTESkeletonTracker& pTracker);
    OINiTESkeletonTracker& operator=(OINiTESkeletonTracker &);
};

class OINiTETrackerFrame : public OITrackerFrame
{
public:
    OINiTETrackerFrame();
    ~OINiTETrackerFrame();

    const std::vector<shared_ptr<OIUserData>> getUsers() override;
    const OIUserData& getUserById(int userId) const override;
    const OIUserMap& getUserMap() const override;
    std::vector<JointType>& getSupportedJointType() override;

private:
    void update(nite::UserTracker* tracker, nite::UserTrackerFrameRef& frameRef);

    OINiTETrackerFrame(const OINiTETrackerFrame &frame);
    OINiTETrackerFrame &operator=(const OINiTETrackerFrame &frame);
};
} // end of namespace

#endif // OPENISS_NITE_SKELETON_TRACKER_H
