#include "OINullFaceTracker.hpp"

#include <iostream>

openiss::OINullFaceTracker::OINullFaceTracker(OIDevice& pDev)
{

	frame_count = 0;
	/*
	// initial for single FacialLandmarks paramters
	singleFacialLandmarksParameters = new LandmarkDetector::FaceModelParameters();
	singleFacialLandmarksParameters->mtcnn_face_detector_location = "/usr/local/etc/OpenFace/model/mtcnn_detector/MTCNN_detector.txt";
	singleFacialLandmarksFaceModel = new LandmarkDetector::CLNF(singleFacialLandmarksParameters->model_location);

	// initial for multi Faciallandmarks paramters
	multiFacialLandmarksParameters = new LandmarkDetector::FaceModelParameters();
	multiFacialLandmarksParameters->mtcnn_face_detector_location = "/usr/local/etc/OpenFace/model/mtcnn_detector/MTCNN_detector.txt";
	// This is so that the model would not try re-initialising itself
	multiFacialLandmarksParameters->reinit_video_every = -1;
	multiFacialLandmarksParameters->curr_face_detector = LandmarkDetector::FaceModelParameters::MTCNN_DETECTOR;
	multiFacialLandmarksParametersVector.clear();
	multiFacialLandmarksParametersVector.push_back(*multiFacialLandmarksParameters);
	*/

	// The modules that are being used for tracking
	int num_faces_max = 4;
	

	/*
	LandmarkDetector::CLNF face_model(multiFacialLandmarksParametersVector[0].model_location);
	if (!face_model.loaded_successfully)
	{
		cout << "ERROR: Could not load the landmark detector" << endl;
		exit(1);
	}
	// Loading the face detectors
	face_model.face_detector_HAAR.load(multiFacialLandmarksParametersVector[0].haar_face_detector_location);
	face_model.haar_face_detector_location = multiFacialLandmarksParametersVector[0].haar_face_detector_location;
	face_model.face_detector_MTCNN.Read(multiFacialLandmarksParametersVector[0].mtcnn_face_detector_location);
	face_model.mtcnn_face_detector_location = multiFacialLandmarksParametersVector[0].mtcnn_face_detector_location;

	// If can't find MTCNN face detector, default to HOG one
	if (multiFacialLandmarksParametersVector[0].curr_face_detector == LandmarkDetector::FaceModelParameters::MTCNN_DETECTOR && face_model.face_detector_MTCNN.empty())
	{
		cout << "INFO: defaulting to HOG-SVM face detector" << endl;
		multiFacialLandmarksParametersVector[0].curr_face_detector = LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR;
	}

	multiFacialLandmarksFaceModelVector.reserve(num_faces_max);
	multiFacialLandmarksFaceModelVector.push_back(face_model);
	*/

	active_models.push_back(false);

	/*
	for (int i = 1; i < num_faces_max; ++i)
	{
		multiFacialLandmarksFaceModelVector.push_back(face_model);
		active_models.push_back(false);
		multiFacialLandmarksParametersVector.push_back(*multiFacialLandmarksParameters);
	}
	*/

	// init FER2D classifier
	//classifier = new FER2D();
	classifier = nullptr;
}

openiss::OINullFaceTracker::OINullFaceTracker()
{
	this->classifier = nullptr;
	this->frame_count = 0;
}



openiss::OINullFaceTracker::~OINullFaceTracker()
{
	/*
	delete singleFacialLandmarksParameters;
	delete singleFacialLandmarksFaceModel;

	delete multiFacialLandmarksParameters;
*/
	delete classifier;
}

#ifdef OPENISS_OPENCV_SUPPORT
void openiss::OINullFaceTracker::drawSingleFaceOnImg(cv::Mat &img)
{
/*
	if (!(*singleFacialLandmarksFaceModel).loaded_successfully)
	{
		cout << "ERROR: Could not load the landmark detector" << endl;
		exit(1);
	}

	if (!(*singleFacialLandmarksFaceModel).eye_model)
	{
		cout << "WARNING: no eye model found" << endl;
		exit(1);
	}
*/

	cv::Mat_<uchar> grayscale_image(img.rows, img.cols);
	cv::cvtColor(img, grayscale_image, CV_BGR2GRAY);

	bool detection_success = true;
	//LandmarkDetector::DetectLandmarksInVideo(img, *singleFacialLandmarksFaceModel,
	//																  *singleFacialLandmarksParameters, grayscale_image);

	// draw points on face
	/*
	cv::Mat_<float> *landmarks = &(singleFacialLandmarksFaceModel->detected_landmarks);

	for (int i = 0; i <= landmarks->rows / 2; i++)
	{
		float x = landmarks->at<float>(0, i);
		float y = landmarks->at<float>(0, i + (int)landmarks->rows / 2);
		cv::Point2f p(x, y);
		cv::circle(img, p, 2, cv::Scalar(0, 0, 250));
	}
	*/

	cv::imshow("asdf", img);
	cv::waitKey(1);
}
#endif

//void NonOverlapingDetections(const vector<LandmarkDetector::CLNF> &clnf_models, vector<cv::Rect_<float>> &face_detections);

#ifdef OPENISS_OPENCV_SUPPORT
void openiss::OINullFaceTracker::drawMultiFacesOnImg(cv::Mat& img)
{
	cv::Mat_<uchar> grayscale_image(img.rows, img.cols);
	cv::cvtColor(img, grayscale_image, CV_BGR2GRAY);

	vector<cv::Rect_<float>> face_detections;
	bool all_models_active = true;

	/*
	for (unsigned int model = 0; model < multiFacialLandmarksFaceModelVector.size(); ++model)
	{
		if (!active_models[model])
		{
			all_models_active = false;
		}
	}
	*/

	// Get the detections (every 8th frame and when there are free models available for tracking)
	if (frame_count % 8 == 0 && !all_models_active)
	{
		/*
		if (multiFacialLandmarksParametersVector[0].curr_face_detector == LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR)
		{
			vector<float> confidences;
			LandmarkDetector::DetectFacesHOG(face_detections, grayscale_image, multiFacialLandmarksFaceModelVector[0].face_detector_HOG, confidences);
		}
		else if (multiFacialLandmarksParametersVector[0].curr_face_detector == LandmarkDetector::FaceModelParameters::HAAR_DETECTOR)
		{
			LandmarkDetector::DetectFaces(face_detections, grayscale_image, multiFacialLandmarksFaceModelVector[0].face_detector_HAAR);
		}
		else
		{
			vector<float> confidences;
			LandmarkDetector::DetectFacesMTCNN(face_detections, img, multiFacialLandmarksFaceModelVector[0].face_detector_MTCNN, confidences);
		}
		*/

		frame_count = 0;
	}

	// Keep only non overlapping detections (so as not to start tracking where the face is already tracked)
	//NonOverlapingDetections(multiFacialLandmarksFaceModelVector, face_detections);
	std::vector<bool> face_detections_used(face_detections.size(), false);

	// Go through every model and update the tracking
	/*
	for (unsigned int model = 0; model < multiFacialLandmarksFaceModelVector.size(); ++model)
	{

		bool detection_success = false;

		// If the current model has failed more than 4 times in a row, remove it
		if (multiFacialLandmarksFaceModelVector[model].failures_in_a_row > 4)
		{
			active_models[model] = false;
			multiFacialLandmarksFaceModelVector[model].Reset();
		}

		// If the model is inactive reactivate it with new detections
		if (!active_models[model])
		{

			for (size_t detection_ind = 0; detection_ind < face_detections.size(); ++detection_ind)
			{
				// if it was not taken by another tracker take it
				if (!face_detections_used[detection_ind])
				{
					face_detections_used[detection_ind] = true;
					// Reinitialise the model
					multiFacialLandmarksFaceModelVector[model].Reset();

					// This ensures that a wider window is used for the initial landmark localisation
					multiFacialLandmarksFaceModelVector[model].detection_success = false;
					detection_success = LandmarkDetector::DetectLandmarksInVideo(img, face_detections[detection_ind], multiFacialLandmarksFaceModelVector[model], multiFacialLandmarksParametersVector[model], grayscale_image);

					// This activates the model
					active_models[model] = true;

					// break out of the loop as the tracker has been reinitialised
					break;
				}
			}
		}
		else
		{
			// The actual facial landmark detection / tracking
			detection_success = LandmarkDetector::DetectLandmarksInVideo(img, multiFacialLandmarksFaceModelVector[model], multiFacialLandmarksParametersVector[model], grayscale_image);
		}

		// draw points on face

		for (int i = 0; i < multiFacialLandmarksFaceModelVector.size(); i++)
		{
			cv::Mat_<float> *landmarks = &(multiFacialLandmarksFaceModelVector[i].detected_landmarks);

			for (int i = 0; i <= landmarks->rows / 2; i++)
			{
				float x = landmarks->at<float>(0, i);
				float y = landmarks->at<float>(0, i + (int)landmarks->rows / 2);
				cv::Point2f p(x, y);
				cv::circle(img, p, 2, cv::Scalar(0, 0, 250));
			}
		}
		

		cv::imshow("MultiFaceLandmarks Detection", img);
		cv::waitKey(1);

		// Update the frame count
		frame_count++;
	}
	*/
}
#endif


void openiss::OINullFaceTracker::initializeFace(openiss::OIFace& p_oFace)
{

	// std::cout << "initial OIFace" << std::endl;

	//cv::Mat image(oiFace.getOriginalImg());

	/*
	 *detect face on img
	 */
	const size_t inWidth = 300;
	const size_t inHeight = 300;
	const double inScaleFactor = 1.0;
	const float confidenceThreshold = 0.7;

	/*
	const cv::Scalar meanVal(104.0, 177.0, 123.0);

	int frameHeight = image.rows;
	int frameWidth = image.cols;

	const std::string caffeConfigFile = "/home/gipsy/Downloads/opencv/samples/dnn/face_detector/deploy.prototxt";
	const std::string caffeWeightFile = "/home/gipsy/Downloads/opencv/samples/dnn/face_detector/res10_300x300_ssd_iter_140000.caffemodel";

	cv::dnn::Net net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);

	cv::Mat inputBlob = cv::dnn::blobFromImage(image, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);

	net.setInput(inputBlob, "data");
	cv::Mat detection = net.forward("detection_out");

	cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());


	for (int i = 0; i < detectionMat.rows; i++)
	{
		float confidence = detectionMat.at<float>(i, 2);

		cv::Mat tempImg(image);
		if (confidence > confidenceThreshold)
		{
			int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
			int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
			int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
			int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);

			cv::Rect myROI(x1, y1, x2 - x1, y2 - y1);

			if (x1 < 0 || x1 >= image.rows || x2 < 0 || x2 >= image.rows ||y1 < 0 || y1 >= image.cols ||y2 < 0 || y2 >= image.cols || x2 - x1 <= 0 || y2 - y1 <= 0) {
				break;
			}
			oiFace.faceVector.push_back(tempImg(myROI));
		}
	}
	*/

	/*
	 * detect face landmarks
	 */

	/*
	for (int i = 0; i < oiFace.faceVector.size(); i++)
	{
		cv::Mat img(oiFace.faceVector[i]);

		/*
		* detect landmarks and save in oiFace.faceVector
		*/

	/*

		if (!(*singleFacialLandmarksFaceModel).loaded_successfully)
		{
			cout << "ERROR: Could not load the landmark detector" << endl;
			exit(1);
		}

		if (!(*singleFacialLandmarksFaceModel).eye_model)
		{
			cout << "WARNING: no eye model found" << endl;
			exit(1);
		}

		cv::Mat_<uchar> grayscale_image(img.rows, img.cols);
		cv::cvtColor(img, grayscale_image, CV_BGR2GRAY);

		bool detection_success = LandmarkDetector::DetectLandmarksInVideo(img, *singleFacialLandmarksFaceModel,
																		  *singleFacialLandmarksParameters, grayscale_image);

		// save landmarks to oiFace
		cv::Mat_<float> *landmarks = &(singleFacialLandmarksFaceModel->detected_landmarks);

		std::vector<cv::Point2f> pointVector;
		for (int i = 0; i <= landmarks->rows / 2; i++)
		{
			float x = landmarks->at<float>(0, i);
			float y = landmarks->at<float>(0, i + (int)landmarks->rows / 2);
			cv::Point2f p(x, y);
			pointVector.push_back(p);
		}
		oiFace.facialLandmarksVector.push_back(pointVector);

	}

	*/

	/*
	 * detect facial expression
	 */
	/*
	for (int i = 0; i < oiFace.faceVector.size(); i++) {
		// cv::Mat outImg;
        // cv::resize(oiFace.faceVector[i], outImg, cv::Size(48, 48), 0, 0, CV_INTER_LINEAR);

		// oiFace.expressionVector.push_back(classifier->predict(outImg));
		oiFace.expressionVector.push_back(classifier->predict(oiFace.faceVector[i]));

	}
	*/
}




//TO DO - Implement properly
openiss::OINullFaceTracker& openiss::OINullFaceTracker::operator=(OINullFaceTracker& copied)
{
	return *this;
}

//Static method to add all the facial landmarks points to a vector of vector of points

void openiss::OINullFaceTracker::resetAdapterParams()
{
	// mSingleFaceParams = false;
	// mMultiFacesParams = false;
}

/*
void NonOverlapingDetections(const vector<LandmarkDetector::CLNF> &clnf_models, vector<cv::Rect_<float>> &face_detections)
{

	// Go over the model and eliminate detections that are not informative (there already is a tracker there)
	for (size_t model = 0; model < clnf_models.size(); ++model)
	{

		// See if the detections intersect
		cv::Rect_<float> model_rect = clnf_models[model].GetBoundingBox();

		for (int detection = face_detections.size() - 1; detection >= 0; --detection)
		{
			double intersection_area = (model_rect & face_detections[detection]).area();
			double union_area = model_rect.area() + face_detections[detection].area() - 2 * intersection_area;

			// If the model is already tracking what we're detecting ignore the detection, this is determined by amount of overlap
			if (intersection_area / union_area > 0.5)
			{
				face_detections.erase(face_detections.begin() + detection);
			}
		}
	}
}
*/

// EOF
