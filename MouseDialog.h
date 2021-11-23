#pragma once

//#include "Timer.h"

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

	float pieSize = 70;
	float x_end_pie = pieSize, y_end_pie = pieSize/2;
	float pieDeg, pieRad;
	CString mStaticInfoText;

	#define UWM_CUSTOMRIGHTCLICK (WM_APP+1)
	bool notDragFlag = true;
	bool notDropFlag = true;
	bool isNonClientArea = false;

	enum ClicksFaces
	{
		LOSTFACE,//GRAY
		NEUTRAL,//WHITE
		SMILING,//GREEN
		DWELL,//GREEN
		ONE_CLICK,//ORANGE
		DOUBLE,//DEEP ORANGE
		CANCEL//RED
	}; 

	ClicksFaces click = NEUTRAL;

	enum Clicks
	{
		ONE_CL,//ORANGE
		DOUBLE_CL,//DEEP ORANGE
		RIGHT_CL,//RED
		DRAG
	};

	Clicks mouseClick = ONE_CL;
	Clicks prevRightClick = ONE_CL;

	static HWND editWnd;


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
	void moveMouseDlg();
	void MouseInput(bool isTimer2Clicks);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
};
