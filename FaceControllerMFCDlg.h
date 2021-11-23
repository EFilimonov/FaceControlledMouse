
// FaceControllerMFCDlg.h : header file
//
#include "Tracker.h"
#include "OptionsDlg.h"
#include "MouseDialog.h"


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui_c.h>


#pragma once


// CFaceControllerMFCDlg dialog
class CFaceControllerMFCDlg : public CDialogEx
{
	// Construction
public:
	CFaceControllerMFCDlg(CWnd* pParent = nullptr);	// standard constructor
	OptionsDlg mOptionsDlg;
	MouseDialog* mMouseDlg;
	PointsTracker tracker;

	TCHAR pathToFolder[MAX_PATH];
	std::string sPathToFolder;

	bool isAutostart = true;
#define ID_TIMER_1 10000
#define ID_TIMER_2 1000
#define ID_TIMER_LOCK 3000
#define ID_TIMER_2CLICKS 200
#define UWM_CUSTOMRIGHTCLICK (WM_APP+1)
#define UWM_CUSTOMDRAGMAIN (WM_APP+4)
#define UWM_CUSTOMDRAGOPTION (WM_APP+5)
#define UWM_CUSTOMSTARTTRACK (WM_APP+6)
#define UWM_CUSTOMCLICK (WM_APP+7)
	CRect clientPosition;
	RECT w;
	int autostartSeconds = 0;
	CString strAutostartSeconds;
	CButton mCheckAutostart;
	CButton mButtonTracking;
	CButton mButtonExit;
	CToolTipCtrl m_ToolTip;
	CRect rectLButton, rectRButton;

	CButton* pButton_click1x;
	CButton* pButton_click2x;
	CButton* pButton_clickRight;
	CButton* pButton_off;
	CButton* pButton_settings;
	CButton* pButton_start;
	CButton* pButton_geo;
	CButton* pButton_ai;
	CButton* pButton_dwell;
	CButton* pButton_timing;
	CButton* pButton_ABC;
	CButton* pButton_Drag;
	CButton* pButton_show;
	CButton* pButton_chrome;

	CStatic* pBigLogoPic;

	CBitmap bmp_click1x;
	CBitmap bmp_click2x;
	CBitmap bmp_clickRight;
	CBitmap bmp_off;
	CBitmap bmp_settings;
	CBitmap bmp_play;
	CBitmap bmp_pause;
	CBitmap bmp_stop;
	CBitmap bmp_quickclick;
	CBitmap bmp_timerclick;
	CBitmap bmp_ABC;
	CBitmap bmp_drag;
	CBitmap bmp_show;
	CBitmap bmp_chrome;

	CBitmap bmp_click1xp;
	CBitmap bmp_click2xp;
	CBitmap bmp_clickRightp;

	CBitmap bmp_geo;
	CBitmap bmp_ai;
	CBitmap bmp_dwell;
	CBitmap bmp_geop;
	CBitmap bmp_aip;
	CBitmap bmp_dwellp;
	CBitmap bmp_dragp;
	CBitmap bmp_showp;

	CBitmap bmp_bigLogo;

	CWnd* startwnd;

	bool isCheckedButtonDrag = true;

	float aspectRatio = 1.80364;
	float newHeight, newWidth, newLeft, newTop;

	bool checkButtonHideFlag = true;

	CRect maxRect;
	int  minHeight, minWidth, btHeight;
	int maxHeight;
	cv::String ek;
	
	bool needAutostart = false;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACECONTROLLERMFC_DIALOG };
#endif

public:

	bool needTrackerInitiate = true;
	bool unlockedStartDragFlag = true;
	POINT startCursorPos;
	int wdx, wdy;
	POINT cursorPos;
	SHELLEXECUTEINFO shExInfo = { 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	void initSettings();

	LRESULT OnOptionsUpdate(WPARAM wparam, LPARAM lparam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOptions();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckAutostart();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedButtonClick1x();
	afx_msg void OnBnClickedButtonClick2x();
	afx_msg void OnBnClickedButtonClickRight();
	afx_msg void OnBnClickedButtonGeo();
	afx_msg void OnBnClickedButtonAi();
	afx_msg void OnBnClickedButtonDwell();
	afx_msg void OnBnClickedButtonTiming();
	afx_msg void OnBnClickedButtonOcr();
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnBnClickedButtonChrome();

	void serializeChanges();
	void readSerialized();
	void resetDefaults();

	void force1Click();

	afx_msg void OnNcMouseLeave();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);

	void resizeCVWindow();
	void minimizeW();
	void maximizeW();

	std::string CFaceControllerMFCDlg::ExePath();
};
