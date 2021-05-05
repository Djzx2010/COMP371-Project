//
// Created by Yiran Shen on 2019-03-10
//

#ifndef OPENISS_OPENFACE_ADAPTER_H
#define OPENISS_OPENFACE_ADAPTER_H


#include <OpenFace/LandmarkCoreIncludes.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>     // solve the problem with imshow and waitkey can not be found
#include <opencv2/imgproc/imgproc.hpp>      // solve the problem with cvtColor can not be found
#include <opencv2/dnn.hpp>

#include <memory>
#include <list>

#include "OIFaceAdapter.hpp"
#include "FER2D.hpp"

using std::shared_ptr;

namespace openiss {

class OIOpenFaceAdapter : public openiss::OIFaceAdapter {
public:
    explicit OIOpenFaceAdapter(OIDevice &pDev);
    ~OIOpenFaceAdapter();

    void drawSingleFaceOnImg(cv::Mat& img) override;
    void drawMultiFacesOnImg(cv::Mat& img) override;
    void resetAdapterParams() override;
    void initializeFace(openiss::OIFace& oiFace) override;


private:
    // disable copy constructor and copy assignment
    OIOpenFaceAdapter(const OIOpenFaceAdapter &pAdapter);
    OIOpenFaceAdapter &operator=(OIOpenFaceAdapter &);

    // sign for Adapter params setup
    bool mSingleFaceParams = false;
    bool mMultiFacesParams = false;

    // parameters for single face adapter
    LandmarkDetector::FaceModelParameters* singleFacialLandmarksParameters;
    LandmarkDetector::CLNF* singleFacialLandmarksFaceModel;

    // parameters for multi face adapter
    LandmarkDetector::FaceModelParameters* multiFacialLandmarksParameters;
    vector<LandmarkDetector::FaceModelParameters> multiFacialLandmarksParametersVector;
    vector<LandmarkDetector::CLNF> multiFacialLandmarksFaceModelVector;
    vector<bool> active_models;
    int frame_count;

    // FER2D facial expression detect
    openiss::FER2D* classifier;


}; // end of class OIOpenFaceAdapter
} // end of namespace

#endif // OPENISS_OPENFACE_ADAPTER_H
