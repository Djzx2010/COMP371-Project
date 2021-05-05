//
// Created by Haotao Lai on 2018-08-08.
//

#ifndef OPENISS_TRACKER_H
#define OPENISS_TRACKER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "OIEnum.hpp"
#include "OIStatus.hpp"

using std::shared_ptr;
using std::vector;

namespace openiss
{
    class OITrackerFrame;
    class OIUserData;
    class OITracker;
    class OISkeletonTracker;
    class OIUserMap;
    class OIFrame;
    class OIDevice;

    class OITrackerFactory
    {
    public:
        shared_ptr<OISkeletonTracker> createTracker(const char* tracName, OIDevice* pDev);
        shared_ptr<OITrackerFrame> createTrackerFrame(const char* tracName);

        ~OITrackerFactory();

    private:
        // TODO: should not be here
        bool mIsNiteInit = false;
    };

/**
 * OITracker provides interface to all kind of trackers.
 * @author Haotao Lai
 * @author Serguei Mokhov
 * @author Jashanjot Singh
 */
class OITracker
{
public:
    virtual ~OITracker() = default;

    /**
     * Initialize as needed devices' and API components.
     */
    virtual OIStatus init() = 0;
    
    /**
     * Gather most recent past data from the sensor frame.
     */
    virtual OIStatus update() = 0;
    
    /**
     * Release memory, resources.
     */
    virtual OIStatus stop() = 0;

    // TODO return OIStatus instead
    virtual void startTracking() = 0;
    virtual void stopTracking() = 0;

    /**
     * @param p_poTrackerFrame -- if not-null the data are filled into the
     * parameter passed. If it is null, the tracker creates the corresponding
     * frame object via the corresponding factory method.
     * 
     * @return a pointer to the new or updated frame; the caller is
     * responsible for freeing its memory.
     */
    virtual OITrackerFrame* readFrame(OITrackerFrame* p_poTrackerFrame = nullptr) = 0;


protected:
    OIStatus m_eStatus = OIStatus::STATUS_TRACKER_NOT_INITIALIZED;

};


/**
 * OITrackerFrame defines the interface of a tracker
 * frame, which should be able to provide a map containing
 * all the users' positions and all the users' data
 * within the current frame.
 */
class OITrackerFrame
{
public:
    virtual ~OITrackerFrame() = default;

    virtual const vector<shared_ptr<OIUserData>> getUsers();
    virtual const OIUserData& getUserById(int userId) const;
    virtual const OIUserMap& getUserMap() const;

    virtual vector<JointType>& getSupportedJointType();

protected:
    std::unordered_map<int, shared_ptr<OIUserData>> mAvailableUsers;
    std::vector<shared_ptr<OIUserData>> mAllUsers;
    std::vector<JointType> supportedJointType;
    
    OIUserMap* mpOIUserMap;

    void prepareSupportedJoints();
};

} // end of namespace

#endif // OPENISS_TRACKER_H
