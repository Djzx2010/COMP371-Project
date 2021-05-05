//
// OISkeleton.hpp
// Skeleton
//
// Created by Haotao Lai on 2018-08-08.
// Created by Jashanjot Singh on 2018-07-22.
//

#ifndef OPENISS_SKELETON_H
#define OPENISS_SKELETON_H

#include <unordered_map>
#include <memory>
#include <vector>
#include "OIEnum.hpp"
#include "OISkeletonTracker.hpp"
#include "OIFrame.hpp"

namespace openiss
{

class OITracker;

/**
 * Point of the skeleton joint (real world coordinate).
 */
class OISkeletonJoint
{
public:
    OISkeletonJoint(){};
    OISkeletonJoint(float x, float y, float z, JointType type,
                    float row=-1.0f, float col=-1.0f);

    JointType type;
    float x, y, z;  // real world coordinates
    float row, col; // image coordinates
};

/**
 * A map associates the JointType to the Joint
 *   JointType -> shared_ptr<SkeletonJoint>
 */
class OISkeleton
{
public:
    std::shared_ptr<openiss::OISkeletonJoint> getJointByType(JointType type);
    void addJointByType(JointType type, std::shared_ptr<OISkeletonJoint> jointPtr);
    void drawToFrame(OIFrame* displayFrame, std::vector<JointType>& supportedJoints);
    void mapWorld2Image(OISkeletonTracker* tracker);
    void setSkeletonState(bool isAvailable);
    bool getSkeletonState() const;

protected:
    std::unordered_map<JointType, std::shared_ptr<OISkeletonJoint>, std::hash<int>> mJoints;
    bool mIsSkeletonAvailable;
};

} // end of namespace

#endif // OPENISS_SKELETON_H
