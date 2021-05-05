//
// Created by Haotao Lai on 2018-08-17.
//

#ifndef OPENISS_USER_H
#define OPENISS_USER_H

#include "OISkeleton.hpp"
#include "OIFace.hpp"
#include "OIGesture.hpp"
#include "OIType.hpp"

namespace openiss
{

/**
 * OIUserMap provides the same frame resolution as the depth
 * frame's, where the background with 0 in that pixel and user
 * id in a specific user's position
 */
class OIUserMap
{
public:
    OIUserMap(int width, int height);
    OIUserMap(int width, int height, short* map);
    ~OIUserMap();

    OIUserMap(const OIUserMap& userMap);
    OIUserMap& operator=(const OIUserMap& otherMap);

    int getHeight() const;
    int getWidth() const;
    short* getPixels() const;

protected:
    int mWidth;
    int mHeight;
    short* mpUserArray;

private:
    void _copy(const OIUserMap &userMap);
};

/**
 * OIUserData contains all the data of a single user
 * within a frame.
 * @author Haotao Lai
 * @author Serguei Mokhov
 */
class OIUserData
{
public:
    explicit OIUserData(long id = 0);
    ~OIUserData();

    OIFace* getFace() const;
    OISkeleton* getSkeleton() const;
    std::vector<OIGestureData> getGestures() const;
    std::vector<OIHandData> getHands() const;
    //xxxx* getActions() const;
    //xxxx* getActivities() const;

    long getID() const;

    Point3f* getBoundingBox();
    Point3f* getCenterOfMass();

protected:
    long mUserId;

    OISkeleton* mpSkeleton;
    OIFace* m_poFace;

    std::vector<OIGestureData> m_aoGestures;
    std::vector<OIHandData> m_aoHands;
    
    Point3f boundingBox[2];
    Point3f centerPoint;

    // TODO: get activities and actions

    // TODO: eigenvector to represent the unique user
};


}
#endif // OPENISS_USER_H
