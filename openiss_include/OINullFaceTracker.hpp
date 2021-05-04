#ifndef OPENISS_NULL_FACE_TRACKER_H
#define OPENISS_NULL_FACE_TRACKER_H

#include <memory>
#include <list>

#include "OIFaceAdapter.hpp"
#include "OIClassifier.hpp"

using std::shared_ptr;

namespace openiss
{

class OINullFaceTracker : public openiss::OIFaceAdapter
{
public:
    explicit OINullFaceTracker(OIDevice& pDev);
    OINullFaceTracker();
    ~OINullFaceTracker();

#ifdef OPENISS_OPENCV_SUPPORT
    void drawSingleFaceOnImg(cv::Mat& img) override;
    void drawMultiFacesOnImg(cv::Mat& img) override;
#endif

    void resetAdapterParams() override;
    void initializeFace(openiss::OIFace& p_oFace) override;


    OINullFaceTracker& operator=(OINullFaceTracker&);

    int frame_count;
    OIClassifier* classifier;
    vector<bool> active_models;

    // disable copy constructor and copy assignment
    OINullFaceTracker(const OINullFaceTracker& pAdapter);


    // sign for Adapter params setup
    bool mSingleFaceParams = false;
    bool mMultiFacesParams = false;

private:
  

    // parameters for single face adapter
    //LandmarkDetector::FaceModelParameters* singleFacialLandmarksParameters;
    //LandmarkDetector::CLNF* singleFacialLandmarksFaceModel;

    // parameters for multi face adapter
    //LandmarkDetector::FaceModelParameters* multiFacialLandmarksParameters;
    //vector<LandmarkDetector::FaceModelParameters> multiFacialLandmarksParametersVector;
    //vector<LandmarkDetector::CLNF> multiFacialLandmarksFaceModelVector;
    

    // FER2D facial expression detect
    //openiss::FER2D* classifier;
    


}; // end of class OINullFaceTracker

} // end of namespace

#endif // OPENISS_NULL_FACE_TRACKER_H
