#include "Timer.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "MouseDialog.h"
#include "Ewma.h"


#pragma once
class Tracker
{
public:
	void geoSmileCalculation(cv::Mat& im, std::vector<cv::Point2f>& landmarks);
	void GetScreenRatio();
	void calclMousePos(cv::Point2f deltaPoint, int faceH);
	void timerMouseDlg();
	void quickMouseDlg();
	void dwellDetecting(float dist);
	void Tracking();
	int getAngleABC(cv::Point2d a, cv::Point2d b, cv::Point2d c);
	cv::Point2f rotate2d(const cv::Point2f& inPoint, const double& angRad);
	cv::Point2f rotatePoint(const cv::Point2f& inPoint, const cv::Point2f& center, const double& angRad);
	cv::Rect calculateCrop(cv::Rect _crop, cv::Size _frame);
	void initiateStart();

	void OnSetMousehook();
	void OnUnhookMouse();

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
	int minFaceNeighbors = 5;
	cv::Ptr<cv::face::Facemark> facemark;
	bool facemarkDetectedFlag = false;
	bool faceDetectedFlag = false;
	bool allowResize = false;

	bool needToTrackerInit = true;

	cv::Point2f trackPoint;
	cv::Point2f trackMousePoint;

	cv::Point2f smileAngle;
	float smilingAnCalcCorr;
	float correctedAng;

	cv::String inptext;
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


#define SMALL_DELTA_LIMIT 300 // small/big movement, bigger -> more sensitive
	double smallDelta;

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

	enum Clicks
	{
		ONE_CL,
		DOUBLE_CL,
		RIGHT_CL,
		DRAG,
		DO_NOTHING
	};

	Clicks mouseClick = DO_NOTHING;

	Timer mouseTimer;
	Timer dwellTimer;
	Timer moveLockTimer;

	bool smileMouseLocked = true;
	bool dwellMouseLocked = true;

	float dwellDisp = 1.0;

	float secDwellStartTime = 2.0;
	float dwellDuration = 0.8;

	float secQuickClick = 0.6;

	float secSmile = 0.6;
	float secOneClick = 0.9;
	float secDoubleClick = 0.9;
	float secCancel = 0.3;

	float secToOneClickDuration;
	float secToDoubleClickDuration;

	float secToOneClickCancelDuration;
	float secToDoubleClickCancelDuration;
	float secDuration = 0;

	bool notDragFlag = true;
	bool notDropFlag = true;
//	bool isNonClientArea = false;
	bool isNonMainClientArea = false;
	bool isNonOptionsClientArea = false;
	bool startDragFlag = false;

	bool lockMoveFlag = true;

	bool needEqualize = true;

	CWnd* m_pMainWnd;
	HWND hWnd;

#define UWM_CUSTOMRIGHTCLICK (WM_APP+1)
#define UWM_CUSTOMDRAGMAIN (WM_APP+4)
#define UWM_CUSTOMDRAGOPTION (WM_APP+5)
#define UWM_CUSTOMSTARTTRACK (WM_APP+6)
	//bool isTimer2Clicks = false;
	cv::Point2f dMouse;

	bool flipCameraFlag = false;
	bool showVideoFlag = true;
	bool use2ThreadsFlag = true;

	int defaultThreadsNum;
	int curThreadsNum = 0;

	Ewma* ewmaXfilter;
	Ewma* ewmaYfilter;
	double ewmaAlpha = 0.6;
	bool turnOnEWMA = true;

	Ewma* ewmaSmileAnglefilter;
	Ewma* ewmasmilingAnCalcCorr;
	double ewmaSmileAlpha = 0.3;

	CString editKeyboard;

	//0.1 Less smoothing - faster to detect changes, but more prone to noise
	//0.01 More smoothing - less prone to noise, but slower to detect changes


// Implementation
protected:


public:


};

