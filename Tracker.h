//#include "Timer.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "MouseDialog.h"
#include "Ewma.h"
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>

#pragma once

class PointMosse
{
	public:
		cv::Rect2d facialRectangle;
		cv::Ptr<cv::legacy::Tracker> facialTracker;
		Ewma facialFilterX;
		Ewma facialFilterY;
		cv::Point2f pointNew;
		cv::Point2f pointOld;
		cv::Point2f pointDelta;
};


class PointsTracker
{
public:
	void initiateTracker();
	void geoSmileCalculation(cv::Mat& im, std::vector<cv::Point2f>& landmarks);
	void GetScreenRatio();
	void calclMousePos(cv::Point2f deltaPoint, int faceH);
	bool Tracking();
	int getAngleABC(cv::Point2d a, cv::Point2d b, cv::Point2d c);
	cv::Point2f rotate2d(const cv::Point2f& inPoint, const double& angRad);
	cv::Point2f rotatePoint(const cv::Point2f& inPoint, const cv::Point2f& center, const double& angRad);
	cv::Rect calculateCrop(cv::Rect _crop, cv::Size _frame);


	void OnSetMousehook();
	void OnUnhookMouse();
	void UnhookMouse();

	void calculateMosseTrackers(cv::Mat& _frame, cv::Mat& _frame_gray);
	bool initFacialTracker(int i, cv::Mat& _frame_gray);
	void ClearEWMAFilters();

	MouseDialog* mMouseDlg;

	POINT cursorPos;
	POINT oldCursorPos;
	POINT newCursorPos;

	float velocityScale;
	float velocitySlider = 3;
	float velocityK = 0.046;

	bool openFlag = true;
	bool turnOffClick = true;

	bool mouseHookPause = false;
	bool buttonStop = true;
	float pauseTime = 5.0;

	CRect buttonStartPosition;

	bool faceInitiatedFlag = false;

	bool loadedFrontalfaceModelFlag = false;
	bool loadedFacemarkModelFlag = false;
	bool loadedSmileModelFlag = false;

	cv::VideoCapture cap;
	cv::CascadeClassifier frontalFaceDetector;
	cv::CascadeClassifier smileDetector;
	CvTermCriteria termcrit;
	int minFaceNeighbors = 5;
	cv::Ptr<cv::face::Facemark> facemark;
	bool facemarkDetectedFlag = false;
	bool faceDetectedFlag = false;
	bool allowResize = false;

	bool firstFaceCropFlag = true;

	cv::Mat frame, frame_gray, faceROI, equalizerROI;
	// Vector of returned faces
	std::vector<cv::Rect> faces;

	std::vector<std::vector<cv::Point2f> > landmarks;


	bool needToTrackerInit = true;

	std::vector<cv::Point2f> pointsT;

	cv::Point2f trackPoint;
	cv::Point2f trackMousePoint;

	cv::Point2f smileAngle;
	float smilingAnCalcCorr;
	float correctedAng;
	Ewma ewmaSmileFilterObj;
	Ewma ewmasmilingAnCalcCorrObj;

	cv::String inptext;
	cv::String facetext = "Face not detected";
	cv::String tempinptext;

#define POINTCOLOR cv::Scalar(0, 79, 241)

#define CORRECTIONCOLOR cv::Scalar(50, 255, 50)

#define CORRECTIONALERTCOLOR cv::Scalar(1, 4, 209)

	float dxTrackPointSum = 0;
	float dyTrackPointSum = 0;
	float dxTrackPoint = 0;
	float dyTrackPoint = 0;
	float olDdDMouseX = 0;
	float olDdDMouseY = 0;

	cv::Rect cropToTrack;
	cv::Rect cropEqualizer;

	std::vector<uchar> status;
	std::vector<float> err;

#define QUALITY_LEVEL  0.001 
	double minDistRatio = 4.0; // cels num per crop (more -> dencer)
	double minDist = 10;

	//#define MAX_CORNERS_COUNT 35 
	//#define MIN_CORNERS_COUNT 10 
	int minCornersCount = 14;
	int maxCornersCount = 25;

#define ULTRASMALLMOVECOLOR cv::Scalar(1, 209, 4)
#define SMALLMOVECOLOR cv::Scalar(1, 209, 4)



#define ULTRASMALL_DELTA_LIMIT 8000 //  ultrasmall/small movement, bigger -> more sensitive 
	double ultraSmallDelta;

	float resultantLenth;
	float addLenth;
	float accumMoveRatio = 0.6;

	bool allowMove;

	float sumVx;
	float sumVy;
#define PI 3.1415926535

	float captureHorizontal;
	float captureVertical;
	float horSensitivity = 1.5;
	float verSensitivity = 1.8;
	float horScale = 8;
	float verScale = 8;
	int horLimit = 0;
	int vertLimit = 0;
	float deltaXpos = 0;
	float deltaYpos = 0;
	int mouseX = 0;
	int mouseY = 0;

	int correctedDX, correctedDY;

	std::vector<cv::Rect> smiles;
	int noseNoseLenth = 1;
	int eyeEyeLenth = 1;
	float rotationHeadRatio = 1;
	int smilingAngle = 60;
	cv::Point corrAB, corrBC;
	int smilingTriggerAngle = 100;
	cv::Rect smileAIrect;

	int minNeighborsSmileDetector = 60;

	bool detectSmileGeoFlag = true;
	bool detectSmileAIFlag = false;
	bool detectDwellFlag = false;
	bool smileGeoDetectedFlag = false;
	bool smileAIDetectedFlag = false;

	bool isQuickClick = false;
	bool quickSmileUnlocked = true;

	Timer moveLockTimer;
	bool skipHook = false;

//	float dwellDisp = 1.0;

	bool isNonOptionsClientArea = false;
	bool isNonMainClientArea = false;

	bool needEqualize = false;

	CWnd* m_pMainWnd;
	HWND hWnd;

#define UWM_CUSTOMRIGHTCLICK (WM_APP+1)
#define UWM_CUSTOMDRAGMAIN (WM_APP+4)
#define UWM_CUSTOMDRAGOPTION (WM_APP+5)
#define UWM_CUSTOMSTARTTRACK (WM_APP+6)
#define UWM_CUSTOMCLICK (WM_APP+7)

	cv::Point2f dMouse;

	bool flipCameraFlag = false;
	bool showVideoFlag = true;

	CString editKeyboard;

	double ewmaAlpha = 0.6;
	double ewmaSmileAlpha = 0.3;
	//0.1 Less smoothing - faster to detect changes, but more prone to noise
	//0.01 More smoothing - less prone to noise, but slower to detect changes

	bool isOpticalFlow;
	bool isMosseTracker;
	float boxWidth;
	float boxHeight;
	float boxRatio = 2.0;

	std::vector<PointMosse> pointsMosse;


	int activeTrackers = 0;
	bool skipframe = true;

// Implementation
protected:


public:


};

