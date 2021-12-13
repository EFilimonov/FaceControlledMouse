#pragma once
#include <xaudio2.h>
#include "Timer.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


// MouseDialog dialog

class MouseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MouseDialog)

public:
	MouseDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MouseDialog();
	CBrush brushGray, brushWhite, brushGreen, brushOrange, brushDeepOrange, brushRed;
	CPen  penGray, penWhite, penGreen, penOrange, penDeepOrange, penRed;
	COLORREF grayCol = RGB(194, 194, 163), 
		whiteCol = RGB(255, 255, 255), 
		greenCol = RGB(0, 153, 0),
		orangeCol = RGB(241, 79, 0),
		deepOrangeCol = RGB(255, 79, 0),
		redCol = RGB(241, 0, 0);

	CBrush* pointerCBrush;
	CPen* pointerCPen;
	POINT cursorPos;
	HBRUSH hbrBkgnd;
	CRect dlgRect;

	float pieSize = 70;
	float x_end_pie = pieSize, y_end_pie = pieSize/2;
	float pieDeg, pieRad;
	CString mStaticInfoText;

#define UWM_CUSTOMRIGHTCLICK (WM_APP+1)
#define UWM_CUSTOMDRAGMAIN (WM_APP+4)
#define UWM_CUSTOMDRAGOPTION (WM_APP+5)
#define UWM_CUSTOMSTARTTRACK (WM_APP+6)
#define UWM_CUSTOMCLICK (WM_APP+7)

	bool notDragFlag = true;
	bool notDropFlag = true;
	bool needSound = true;
	bool isNonMainClientArea = false;
	bool isNonOptionsClientArea = false;
	bool isTimer2Clicks = false;

	enum ClicksFaces
	{
		LOSTFACE,//GRAY
		NEUTRAL,//WHITE
		SMILING,//GREEN
		DWELL,//GREEN
		ONE_CLICK,//ORANGE
		DOUBLE,//DEEP ORANGE
		CANCEL,//RED
		PAUSE
	}; 

	ClicksFaces click = NEUTRAL;

	enum Clicks
	{
		ONE_CL,//ORANGE
		DOUBLE_CL,//DEEP ORANGE
		RIGHT_CL,//RED
		DRAG,
		DO_NOTHING
	};

	Clicks mouseClick = ONE_CL;
	Clicks prevRightClick = ONE_CL;


	CWnd* m_pMainWnd;
	HWND hWnd;

	static HWND editWnd;

	HRESULT hr;
	IXAudio2SourceVoice* pSourceVoice;
	XAUDIO2_BUFFER buffer = { 0 };
	bool playSoundFlag = true;

	Timer mouseTimer;
	Timer dwellTimer;

	double elapsedSeconds;

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

	bool isQuickClick = false;
	bool quickSmileUnlocked = true;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void rotatePie(double elapsedSeconds, bool isDwell, bool isQuick, float _secSmile, float _sec1x, float _sec2x, float _secAll);
	void changePie(ClicksFaces _click);
	void moveMouseDlg(bool onTop);

	void timerMouseDlg(bool detect);
	void quickMouseDlg(bool detect);
	void dwellDetecting(float dist);
	void initiateStart();
	void stopTimers();
	void MouseInput();
	void playClickSound();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
};
