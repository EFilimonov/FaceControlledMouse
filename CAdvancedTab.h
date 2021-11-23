#pragma once
#include "AboutDialog.h"

// CAdvancedTab dialog

class CAdvancedTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAdvancedTab)

public:
	CAdvancedTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAdvancedTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADVANCED_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	AboutDialog mAboutDialog;



	#define UWM_UPDATE_OPTIONS (WM_APP+2)
	CString str1, str2;
	CWnd* m_pMainWnd;
	HWND hWnd;

	CButton mCheckFlipCamera;
	CButton mCheckShowVideo;
	CButton mCheck2Threads;
	CButton mCheckEWMA;

	CSliderCtrl mSlider2Threads;
	CSliderCtrl mSliderEWMA;
	CSliderCtrl mSliderMarks;
	CSliderCtrl mSliderAcceleration;
	CSliderCtrl mSliderFace;

	CStatic mStaticEWMA;
	CStatic mStaticMarks;
	CStatic mStaticAcceleration;
	CStatic mStaticFace;
	CStatic mStaticThreadsNum;

	bool flipCameraFlag = false;
	bool showVideoFlag = true;
	bool use2ThreadsFlag = true;
	bool EWMAFlag = true;
	float iSliderEWMA;
	float iSliderMarks;
	float iSliderAcceleration;
	int iSliderFace = 5;
	int defaultThreadsNum = 0;
	int curThreadsNum = 0;

	afx_msg void OnBnClickedCheckFlipcamera();
	afx_msg void OnBnClickedCheckShowvideo();
	afx_msg void OnBnClickedCheck2threads();
	afx_msg void OnBnClickedCheckEwma();
	afx_msg void OnNMCustomdrawSliderEwma(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderAcceleration(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFaceSensitivity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider2threads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonAbout();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

};
