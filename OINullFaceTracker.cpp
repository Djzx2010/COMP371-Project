#include "OINullFaceTracker.hpp"

#include <iostream>

openiss::OINullFaceTracker::OINullFaceTracker(OIDevice& pDev)
{
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
	this->oiface = new OIFace();
	this->currentFace = 6;
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
	delete oiface;
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


void openiss::OINullFaceTracker::generateFaces()
{
	//Generating all the hardcoded points
	//P1
	openiss::Point2f p1;
	p1.x = -2.5f;
	p1.y = 5.25f;
	//P2
	openiss::Point2f p2;
	p2.x = -2.48f;
	p2.y = 4.6f;
	//P3
	openiss::Point2f p3;
	p3.x = -2.45f;
	p3.y = 4.0f;
	//P4
	openiss::Point2f p4;
	p4.x = -2.42f;
	p4.y = 3.5f;
	//P5
	openiss::Point2f p5;
	p5.x = -2.3f;
	p5.y = 3.2f;
	//P6
	openiss::Point2f p6;
	p6.x = -2.2f;
	p6.y = 2.75f;
	//P7
	openiss::Point2f p7;
	p7.x = -1.8f;
	p7.y = 2.4f;
	//P8
	openiss::Point2f p8;
	p8.x = -1.3f;
	p8.y = 1.6f;
	//P9
	openiss::Point2f p9;
	p9.x = -0.75f;
	p9.y = 1.0f;
	//P10
	openiss::Point2f p10;
	p10.x = 0.0f;
	p10.y = 0.9f;
	//P11
	openiss::Point2f p11;
	p11.x = 0.75f;
	p11.y = 1.0f;
	//P12
	openiss::Point2f p12;
	p12.x = 1.3f;
	p12.y = 1.6f;
	//P13
	openiss::Point2f p13;
	p13.x = 1.8f;
	p13.y = 2.4f;
	//P14
	openiss::Point2f p14;
	p14.x = 2.0f;
	p14.y = 2.75f;
	//P15
	openiss::Point2f p15;
	p15.x = 2.3f;
	p15.y = 3.2f;
	//P16
	openiss::Point2f p16;
	p16.x = 2.42f;
	p16.y = 3.5f;
	//P17
	openiss::Point2f p17;
	p17.x = 2.45f;
	p17.y = 4.0f;
	//P18
	openiss::Point2f p18;
	p18.x = 2.45f;
	p18.y = 4.0f;
	//P19
	openiss::Point2f p19;
	p19.x = 2.5f;
	p19.y = 5.25f;
	//P20
	openiss::Point2f p20;
	p20.x = 0.0f;
	p20.y = 8.5f;
	//P21
	openiss::Point2f p21;
	p21.x = -2.0f;
	p21.y = 4.6f;
	//P22
	openiss::Point2f p22;
	p22.x = -2.0f;
	p22.y = 6.4f;
	//P23
	openiss::Point2f p23;
	p23.x = -1.5f;
	p23.y = 6.5f;
	//P24
	openiss::Point2f p24;
	p24.x = -1.0f;
	p24.y = 6.5f;
	//P25
	openiss::Point2f p25;
	p25.x = -0.5f;
	p25.y = 6.2f;
	//P26
	openiss::Point2f p26;
	p26.x = -1.0f;
	p26.y = 6.1f;
	//P27
	openiss::Point2f p27;
	p27.x = -1.5f;
	p27.y = 6.1f;
	//P28
	openiss::Point2f p28;
	p28.x = -2.0f;
	p28.y = 6.05f;
	//P29
	openiss::Point2f p29;
	p29.x = 2.0f;
	p29.y = 4.6f;
	//P30
	openiss::Point2f p30;
	p30.x = 2.0f;
	p30.y = 6.4f;

	openiss::Point2f p31;
	p31.x = 1.5;
	p31.y = 6.5;

	openiss::Point2f p32;
	p32.x = 1.0;
	p32.y = 6.5;

	openiss::Point2f p33;
	p33.x = 0.5;
	p33.y = 6.2;

	openiss::Point2f p34;
	p34.x = 1.0;
	p34.y = 6.1;

	openiss::Point2f p35;
	p35.x = 1.5;
	p35.y = 6.1;

	openiss::Point2f p36;
	p36.x = 2.0;
	p36.y = 6.05;

	openiss::Point2f p37;
	p37.x = -1.8;
	p37.y = 5.6;

	openiss::Point2f p38;
	p38.x = -1.3;
	p38.y = 5.7;

	openiss::Point2f p39;
	p39.x = -1.1;
	p39.y = 5.6;

	openiss::Point2f p40;
	p40.x = -1.1;
	p40.y = 5.7;

	openiss::Point2f p41;
	p41.x = -1.1;
	p41.y = 5.8;

	openiss::Point2f p42;
	p42.x = -0.9;
	p42.y = 5.7;

	openiss::Point2f p43;
	p43.x = -0.7;
	p43.y = 5.6;

	openiss::Point2f p44;
	p44.x = -0.5;
	p44.y = 5.6;

	openiss::Point2f p45;
	p45.x = 1.8;
	p45.y = 5.6;

	openiss::Point2f p46;
	p46.x = 1.3;
	p46.y = 5.7;

	openiss::Point2f p47;
	p47.x = 1.1;
	p47.y = 5.6;

	openiss::Point2f p48;
	p48.x = 1.1;
	p48.y = 5.7;

	openiss::Point2f p49;
	p49.x = 1.1;
	p49.y = 5.8;

	openiss::Point2f p50;
	p50.x = 0.9;
	p50.y = 5.7;

	openiss::Point2f p51;
	p51.x = 0.7;
	p51.y = 5.6;

	openiss::Point2f p52;
	p52.x = 0.5;
	p52.y = 5.6;

	openiss::Point2f p53;
	p53.x = -0.6;
	p53.y = 4.0;

	openiss::Point2f p54;
	p54.x = -0.9;
	p54.y = 4.2;

	openiss::Point2f p55;
	p55.x = -0.7;
	p55.y = 4.6;

	openiss::Point2f p56;
	p56.x = 0.0;
	p56.y = 3.6;

	openiss::Point2f p57;
	p57.x = 0.6;
	p57.y = 4.0;

	openiss::Point2f p58;
	p58.x = 0.9;
	p58.y = 4.2;

	openiss::Point2f p59;
	p59.x = 0.7;
	p59.y = 4.6;

	// Smile
	openiss::Point2f p60s;
	p60s.x = -1.3;
	p60s.y = 3.2;

	openiss::Point2f p61s;
	p61s.x = 1.3;
	p61s.y = 3.2;

	openiss::Point2f p62s;
	p62s.x = -0.7;
	p62s.y = 3.1;

	openiss::Point2f p63s;
	p63s.x = -0.7;
	p63s.y = 2.6;

	openiss::Point2f p64s;
	p64s.x = -0.9;
	p64s.y = 2.4;

	openiss::Point2f p65s;
	p65s.x = -0.7;
	p65s.y = 3.1;

	openiss::Point2f p66s;
	p66s.x = 0.7;
	p66s.y = 2.6;

	openiss::Point2f p67s;
	p67s.x = 0.9;
	p67s.y = 2.4;

	openiss::Point2f p68s;
	p68s.x = -0.1;
	p68s.y = 3.1;

	openiss::Point2f p69s;
	p69s.x = 0.1;
	p69s.y = 3.1;

	openiss::Point2f p70s;
	p70s.x = -0.3;
	p70s.y = 0.5;

	openiss::Point2f p71s;
	p71s.x = 0;
	p71s.y = 2.5;

	openiss::Point2f p72s;
	p72s.x = 0;
	p72s.y = 2.3;


	// Neutral
	openiss::Point2f p60n;
	p60n.x = -1.3;
	p60n.y = 2.7;

	openiss::Point2f p61n;
	p61n.x = 1.3;
	p61n.y = 2.7;

	openiss::Point2f p62n;
	p62n.x = -0.7;
	p62n.y = 2.9;

	openiss::Point2f p63n;
	p63n.x = -0.65;
	p63n.y = 2.75;

	openiss::Point2f p64n;
	p64n.x = -0.7;
	p64n.y = 2.6;

	openiss::Point2f p65n;
	p65n.x = 0.7;
	p65n.y = 2.9;

	openiss::Point2f p66n;
	p66n.x = 0.65;
	p66n.y = 2.75;

	openiss::Point2f p67n;
	p67n.x = 0.7;
	p67n.y = 2.6;

	openiss::Point2f p68n;
	p68n.x = -0.1;
	p68n.y = 3.0;

	openiss::Point2f p69n;
	p69n.x = 0.1;
	p69n.y = 3.0;

	openiss::Point2f p70n;
	p70n.x = 0;
	p70n.y = 2.95;

	openiss::Point2f p71n;
	p71n.x = 0;
	p71n.y = 2.75;

	openiss::Point2f p72n;
	p72n.x = 0;
	p72n.y = 2.4;


	// Frown
	openiss::Point2f p60f;
	p60f.x = -1.2;
	p60f.y = 2.65;

	openiss::Point2f p61f;
	p61f.x = 1.2;
	p61f.y = 2.65;

	openiss::Point2f p62f;
	p62f.x = -0.5;
	p62f.y = 3.0;

	openiss::Point2f p63f;
	p63f.x = -0.45;
	p63f.y = 2.90;

	openiss::Point2f p64f;
	p64f.x = -0.45;
	p64f.y = 2.65;

	openiss::Point2f p65f;
	p65f.x = 0.5;
	p65f.y = 3.0;

	openiss::Point2f p66f;
	p66f.x = 0.45;
	p66f.y = 2.90;

	openiss::Point2f p67f;
	p67f.x = 0.45;
	p67f.y = 2.65;

	openiss::Point2f p68f;
	p68f.x = -0.2;
	p68f.y = 3.1;

	openiss::Point2f p69f;
	p69f.x = 0.2;
	p69f.y = 3.1;

	openiss::Point2f p70f;
	p70f.x = 0;
	p70f.y = 3.05;

	openiss::Point2f p71f;
	p71f.x = 0;
	p71f.y = 2.95;

	openiss::Point2f p72f;
	p72f.x = 0;
	p72f.y = 2.6;

	//Array of points common to every face
	openiss::Point2f basePoints[] = {p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,
		p28,p29,p30,p31,p32,p33,p34,p35,p36,p37,p38,p39,p40,p41,p42,p43,p44,p45,p46,p47,p48,p49,p50,p51,p52,p53,p54,p55,p56,p57,p58,p59};

	//Array of points for the mouth (smile)
	openiss::Point2f smilePoints[] = {p60s,p61s,p62s,p63s,p64s,p65s,p66s,p67s,p68s,p69s,p70s,p71s,p72s};

	//Array of points for the mouth (neutral)
	openiss::Point2f neutralPoints[] = { p60n,p61n,p62n,p63n,p64n,p65n,p66n,p67n,p68n,p69n,p70n,p71n,p72n };
	
	//Array of points for the mouth (frown)
	openiss::Point2f frownPoints[] = { p60f,p61f,p62f,p63f,p64f,p65f,p66f,p67f,p68f,p69f,p70f,p71f,p72f};
	
	for (int i = 0; i < 7; i++) {
		
		vector<openiss::Point2f>* newVector = new vector<openiss::Point2f>;

		oiface->facialLandmarks->push_back(*newVector);
		
		oiface->facialLandmarks->at(i).insert(oiface->facialLandmarks->at(i).begin(), basePoints, basePoints + 59);
		
		if (i == 0) {
			for (int j = 0; j < 13; j++) {
				oiface->facialLandmarks->at(i).push_back(smilePoints[j]);
			}
		}
		else if (i == 1) {
			for (int j = 0; j < 13; j++) {
				openiss::Point2f point;
				point.x = smilePoints[j].x + (neutralPoints[j].x - smilePoints[j].x) / 3;
				point.y = smilePoints[j].y + (neutralPoints[j].y - smilePoints[j].y) / 3;
				oiface->facialLandmarks->at(i).push_back(point);
			}
		}
		else if (i == 2) {
			for (int j = 0; j < 13; j++) {
				openiss::Point2f point;
				point.x = smilePoints[j].x + (neutralPoints[j].x - smilePoints[j].x)*2 / 3;
				point.y = smilePoints[j].y + (neutralPoints[j].y - smilePoints[j].y)*2 / 3;
				oiface->facialLandmarks->at(i).push_back(point);
			}
		}
		else if (i == 3) {
			for (int j = 0; j < 13; j++) {
				oiface->facialLandmarks->at(i).push_back(neutralPoints[j]);
			}
		}
		else if (i == 4) {
			for (int j = 0; j < 13; j++) {
				openiss::Point2f point;
				point.x = neutralPoints[j].x + (frownPoints[j].x - neutralPoints[j].x) / 3;
				point.y = neutralPoints[j].y + (frownPoints[j].y - neutralPoints[j].y) / 3;
				oiface->facialLandmarks->at(i).push_back(point);
			}
		}
		else if (i == 5) {
			for (int j = 0; j < 13; j++) {
				openiss::Point2f point;
				point.x = neutralPoints[j].x + (frownPoints[j].x - neutralPoints[j].x) *2/ 3;
				point.y = neutralPoints[j].y + (frownPoints[j].y - neutralPoints[j].y) *2/ 3;
				oiface->facialLandmarks->at(i).push_back(point);
			}
		}
		else if (i == 6) {
			for (int j = 0; j < 13; j++) {
				oiface->facialLandmarks->at(i).push_back(frownPoints[j]);
			}
		}
	}
}

vector<openiss::Point2f> openiss::OINullFaceTracker::getNextFace()
{
	if (currentFace == 6) {
		currentFace = 0;
	}
	else {
		currentFace += 1;
	}
	return oiface->facialLandmarks->at(currentFace);
}


//TO DO - Implement properly
openiss::OINullFaceTracker& openiss::OINullFaceTracker::operator=(OINullFaceTracker& copied)
{
	this->currentFace = copied.currentFace;
	this->oiface = copied.oiface;
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
