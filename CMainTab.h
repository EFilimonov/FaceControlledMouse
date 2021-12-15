#pragma once


// CMainTab dialog

class CMainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab)

public:
	CMainTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	#define UWM_UPDATE_OPTIONS (WM_APP+2) 
	CString str1, str2;
	CWnd* m_pMainWnd;
	HWND hWnd;

	CSliderCtrl mSliderSpeedHorisontal;
	CSliderCtrl mSliderSpeedVertical;
	CSliderCtrl mSliderSmileAngle;
	CSliderCtrl mSliderSmileSensitivity;
	CSliderCtrl mSliderDwellDispl;
	CSliderCtrl mSliderDwellTime;
	CSliderCtrl mSliderQuickTime;
	CSliderCtrl mSlider1x;
	CSliderCtrl mSlider2x;
	CSliderCtrl mSliderCancel;
	CSliderCtrl mSliderTimeToDwell;


	float fSliderSpeedHorisontal;
	float fSliderSpeedVertical;
	int iSliderSmileAngle;
	int iSliderSmileSensitivity;
	float fSliderDwellDispl;
	float fSliderDwellTime;
	float fSliderQuickTime;
	float fSlider1x;
	float fSlider2x;
	float fSliderCancel;
	float fSliderTimeToDwell;

	CStatic mStaticSpeedHorizontal;
	CStatic mStaticSpeedVertical;
	CStatic mStaticSmileAngle;
	CStatic mStaticSmileSensitivity;
	CStatic mStaticDwellDisp;
	CStatic mStaticDwellTime;
	CStatic mStaticQuickTime;
	CStatic mStatic1x;
	CStatic mStatic2x;
	CStatic mStaticCancel;
	CStatic mStaticTimeToDwell;
	CStatic mStaticMouseSpeed;
	CStatic mStaticDwell;

	CString statictext3 = L"Mouse speed";
	CString statictext4 = L"Horizontal mouse speed (X): ";
	CString statictext5 = L"Vertical mouse speed (Y): ";
	CString statictext6 = L"Smile angle (geometric): ";
	CString statictext7 = L"Recognition threshold (AI): ";
	CString statictext8 = L"Quick click time, (sec): ";
	CString statictext9 = L"Long click timing, (sec)";
	CString statictext10 = L"Waiting: ";
	CString statictext11 = L"1x click: ";
	CString statictext12 = L"2x clicks: ";
	CString statictext13 = L"Dwell parameters";
	CString statictext14 = L"Dwell displacement: ";
	CString statictext15 = L"Time to dwell, (sec): ";
	CString statictext16 = L"Dwell duration, (sec): ";

	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSliderSpeedHorizontal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderSpeedVertical(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderSmileAngle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderSmileSensitivity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderDwellDispl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderDwellTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderQuickTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider1x(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider2x(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderCancel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderTimetodwell(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();

};
