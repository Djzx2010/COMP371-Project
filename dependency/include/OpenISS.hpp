/**
 * Main OpenISS entry API.
 *
 * @author Serguei Mokhov
 * @author Includes: Haotao Lai on 2018-08-10.
 */

#ifndef OPENISS_H
#define OPENISS_H

#include "Config.h"

#include "OIDevice.hpp"
#include "OISkeleton.hpp"
#include "OIFace.hpp"
#include "OIFaceAdapter.hpp"
#include "OIFrame.hpp"
#include "OITracker.hpp"
#include "OIUser.hpp"
#include "OIGesture.hpp"
#include "OIClassifier.hpp"

namespace openiss
{

typedef enum
{
    NULL_TRACKER,
    PERSON_TRACKER,
    GESTURE_RECOGNIZER,
    FACE_TRACKER,
    HAND_TRACKER
} e_trackers;

// Select / enum face tracker backends

typedef enum
{
    NULL_DEVICE,
    ADAPTER_RGB_CAMERA,
    ADAPTER_OPENNI2,
    ADAPTER_LIBFREENECT,
    ADAPTER_LIBFREENECT2,
    ADAPTER_NUITRACK,
    ADAPTER_REALSENSE_SDK2,
    ADAPTER_STRUCTURE_SDK,
    ADAPTER_AZURE_KINECT_SDK,
    ADAPTER_ROS
} e_sensor_adapter;

class OpenISS
{
    protected:

        /**
         * Current and default device.
         */
        std::shared_ptr<OIDevice> m_oSensorAdapter;

        /**
         * A list of current devices,
         */
        std::vector<std::shared_ptr<OIDevice>> m_paoDevices;

        /**
         * A list of current trackers,
         */
        std::vector<OITracker*> m_paoTrackers;

        /**
         * A list of current classifiers and detectors,
         */
        std::vector<OIClassifier*> m_paoClassifiers;

        OIDeviceFactory m_oDeviceFactory;
        OITrackerFactory m_oTrackerFactory;
        OIFaceAdapterFactory m_oFaceTrackerFactory;
        //OIGestureTrackerFactory m_oGestureTrackerFactory;
        //OIClassifierFactory m_oClassifierFactory;

        bool m_bDepthEnabled = false;
        bool m_bRGBEnabled = false;
        bool m_bIREnabled = false;

        bool m_bSkeletonTrackingEnabled = false;
        bool m_bFaceTrackingEnabled = false;
        bool m_bHandTrackingEnabled = false;
        bool m_bGestureTrackingEnabled = false;
        bool m_bActivityTrackingEnabled = false;
        bool m_bActionTrackingEnabled = false;

    public:

        /**
         * Default constructor instantiates a null device
         * and a null skeleton tracker.
         */
        OpenISS();

        /**
         * Instantiates an OpenISS context instance with
         * aspecified type of sensor device adapter.
         * @param p_eAdapterType -- the adapter type wanted
         * @see openiss::e_sensor_adapter
         */
        OpenISS(e_sensor_adapter p_eAdapterType);
        
        OpenISS
        (
            e_sensor_adapter p_eAdapterType,
            bool p_bEnablDepth,
            bool p_bEnableRGB,
            bool p_bEnableIR,
            bool p_bEnableSkeleton,
            bool p_bEnableGestures,
            bool p_bEnableHands,
            bool p_bEnableFaces
        );

        virtual ~OpenISS();

        // TODO: add Configuration object and env vars
        OIStatus init();

        int enumerateDevices();
        int getDeviceCount();

        std::shared_ptr<OIDevice> getCurrentDevice();
        std::shared_ptr<OIDevice> getDevice(int = 0);
        std::vector<std::shared_ptr<OIDevice>> getAllDevices();
        
        OIFrame* getDepthFrame(OIDevice* = nullptr);
        OIFrame* getRGBFrame(OIDevice* = nullptr);
        OIFrame* getIRFrame(OIDevice* = nullptr);
        OIFrame* getGreenScreenedFrame(OIDevice* = nullptr);

        OITrackerFrame* getUserFrame(OITracker* = nullptr);
        const OIUserMap* getUserMap(OITracker* = nullptr);
        std::vector<OIUserData> getUsers(OITracker* = nullptr);
        std::vector<OISkeleton> getSkeletons(OITracker* = nullptr);
        OIFace** getFaces(OITracker* = nullptr);

        //OIPointCloud** getPointCloud(OIDevice* = nullptr);
        //OIPointCloud** getDecoratedPointedCloud(OIDevice* = nullptr);

        std::vector<OIGestureData> getGestures(OITracker* = nullptr);
        std::vector<OIHandData> getHands(OITracker* = nullptr);

        //OIActivity** getActivities(OITracker* = nullptr);
        //OIAction** getActions(OITracker* = nullptr);

        OIStatus enableDepth(bool = true, OIDevice* = nullptr);
        OIStatus enableRGB(bool = true, OIDevice* = nullptr);
        OIStatus enableIR(bool = true, OIDevice* = nullptr);

        /**
         * Enable/disable skeleton tracking at the specified tracker.
         * If the tracker is omitted or NULL, all skeleton trackers are affected.
         * If the tracker is not a skeleton tracker an error status is returned.
         * 
         * @param p_bEnable -- true or false
         * @param p_poTracker -- specific tracker to enable/disable skeleton tracking at
         * @return status
         */
        OIStatus enableSkeletonTracking(bool p_bEnable = true, OITracker* p_poTracker = nullptr);

        OIStatus enableGestureTracking(bool = true, OITracker* = nullptr);
        OIStatus enableHandTracking(bool = true, OITracker* = nullptr);
        OIStatus enableFaceTracking(bool = true, OITracker* = nullptr);

        // TODO
        //OIStatus enableActionTracking(bool p_bEnable = true, OITracker* p_poTracker = nullptr);
        //OIStatus enableActivityDetection(bool p_bEnable = true, OITracker* p_poTracker = nullptr);
}; // class OpenISS

} // namespace

#endif // OPENISS_H
