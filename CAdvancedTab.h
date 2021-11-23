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
//	CButton mCheckShowVideo;
	CButton mCheck2Threads;
	CButton mCheckEWMA;
	CButton mCheckEqualize;
	CButton mCheckAutostart;

	CSliderCtrl mSlider2Threads;
	CSliderCtrl mSliderEWMA;
	CSliderCtrl mSliderMarks;
	CSliderCtrl mSliderAcceleration;
	CSliderCtrl mSliderFace;
	CSliderCtrl mSliderFaceMinNum;
	CSliderCtrl mSliderFaceMaxNum;
	CSliderCtrl mSliderCustom;

	CStatic mStaticMinNum;
	CStatic mStaticMaxNum;
	CStatic mStaticEWMA;
	CStatic mStaticMarks;
	CStatic mStaticAcceleration;
	CStatic mStaticFace;
	CStatic mStaticThreadsNum;
	CStatic mStaticCustom;

	CString mEditKeyboard;

	bool flipCameraFlag = false;
	bool needEqualize = true;
	bool use2ThreadsFlag = true;
	bool EWMAFlag = true;
	float iSliderEWMA;
	float iSliderMarks;
	float iSliderAcceleration;
	int iSliderFace = 5;
	int iSliderMinNum = 12;
	int iSliderMaxNum = 30;
	int defaultThreadsNum = 0;
	int curThreadsNum = 0;
	int iSliderCustom = 10;
	bool needAutostart = false;

	afx_msg void OnBnClickedCheckFlipcamera();
	//afx_msg void OnBnClickedCheckShowvideo();
	afx_msg void OnBnClickedCheck2threads();
	afx_msg void OnBnClickedCheckEwma();
	afx_msg void OnNMCustomdrawSliderEwma(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderAcceleration(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFaceSensitivity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider2threads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnEnChangeEditKeyboard();
	afx_msg void OnBnClickedCheckEqualize();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	afx_msg void OnNMCustomdrawSliderFaceMinnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFaceMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderCustom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckAutostart();
	BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName);
	BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
	void RegisterProgram();
	LONG DeleteMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
	void DeleteProgram();


};
