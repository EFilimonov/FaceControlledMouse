#pragma once
#include "CMainTab.h"
#include "CAdvancedTab.h"
#include "MouseDialog.h"

// OptionsDlg dialog

class OptionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OptionsDlg)


public:
	OptionsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OptionsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS };
#endif

#define UWM_UPDATE_OPTIONS (WM_APP+2)
	CWnd* m_pMainWnd;
	HWND hWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TC_ITEM TabItem;

	CTabCtrl m_ctrTab;
	CMainTab pCMainTab;
	CAdvancedTab pCAdvancedTab;
	MouseDialog* mMouseDlg;

	CToolTipCtrl m_ToolTip;

	float back_fSliderSpeedHorisontal;
	float back_fSliderSpeedVertical;
	int back_iSliderSmileAngle;
	int back_iSliderSmileSensitivity;
	float back_fSliderDwellDispl;
	float back_fSliderDwellTime;
	float back_fSliderQuickTime;
	float back_fSlider1x;
	float back_fSlider2x;
	float back_fSliderCancel;
	float back_fSliderTimeToDwell;
	CString back_editKeyboard;
	bool back_needSound;

	bool back_flipCameraFlag;
	bool back_needEqualize;
	float back_fSliderEWMA;
//	float back_fSliderMarks;
	float back_fSliderAcceleration;
	float back_fSliderOnInput;
	int back_iSliderFace;
	int back_minFaceNeighbors;

	CString back_mEditKeyboard;

	LPWSTR statictext1 = L"Main";
	LPWSTR statictext2 = L"Advanced";

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNcMouseLeave();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
};
