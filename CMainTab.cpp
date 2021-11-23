// CMainTab.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "CMainTab.h"
#include "afxdialogex.h"


// CMainTab dialog

IMPLEMENT_DYNAMIC(CMainTab, CDialogEx)

CMainTab::CMainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{

}

CMainTab::~CMainTab()
{
}



void CMainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SPEED_HORIZONTAL, mSliderSpeedHorisontal);
	DDX_Control(pDX, IDC_SLIDER_SPEED_VERTICAL, mSliderSpeedVertical);
	DDX_Control(pDX, IDC_SLIDER_SMILE_ANGLE, mSliderSmileAngle);
	DDX_Control(pDX, IDC_SLIDER_SMILE_SENSITIVITY, mSliderSmileSensitivity);
	DDX_Control(pDX, IDC_SLIDER_DWELL_DISPL, mSliderDwellDispl);
	DDX_Control(pDX, IDC_SLIDER_DWELL_TIME, mSliderDwellTime);
	DDX_Control(pDX, IDC_SLIDER_QUICK_TIME, mSliderQuickTime);
	DDX_Control(pDX, IDC_SLIDER_1X, mSlider1x);
	DDX_Control(pDX, IDC_SLIDER_2X, mSlider2x);
	DDX_Control(pDX, IDC_SLIDER_CANCEL, mSliderCancel);
	DDX_Control(pDX, IDC_SLIDER_TIMETODWELL, mSliderTimeToDwell);

	DDX_Control(pDX, IDC_STATIC_SPEED_HORIZONTAL, mStaticSpeedHorizontal);
	DDX_Control(pDX, IDC_STATIC_SPEED_VERTICAL, mStaticSpeedVertical);
	DDX_Control(pDX, IDC_STATIC_SMILE_ANGLE, mStaticSmileAngle);
	DDX_Control(pDX, IDC_STATIC_SMILE_SENSITIVITY, mStaticSmileSensitivity);
	DDX_Control(pDX, IDC_STATIC_DWELL_DISPL, mStaticDwellDisp);
	DDX_Control(pDX, IDC_STATIC_DWELL_TIME, mStaticDwellTime);
	DDX_Control(pDX, IDC_STATIC_QUICK_TIME, mStaticQuickTime);
	DDX_Control(pDX, IDC_STATIC_1X, mStatic1x);
	DDX_Control(pDX, IDC_STATIC_2X, mStatic2x);
	DDX_Control(pDX, IDC_STATIC_CANCEL, mStaticCancel);
	DDX_Control(pDX, IDC_STATIC_TIMETODWELL, mStaticTimeToDwell);

}


BEGIN_MESSAGE_MAP(CMainTab, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SPEED_HORIZONTAL, &CMainTab::OnNMCustomdrawSliderSpeedHorizontal)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SPEED_VERTICAL, &CMainTab::OnNMCustomdrawSliderSpeedVertical)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SMILE_ANGLE, &CMainTab::OnNMCustomdrawSliderSmileAngle)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SMILE_SENSITIVITY, &CMainTab::OnNMCustomdrawSliderSmileSensitivity)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DWELL_DISPL, &CMainTab::OnNMCustomdrawSliderDwellDispl)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DWELL_TIME, &CMainTab::OnNMCustomdrawSliderDwellTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_QUICK_TIME, &CMainTab::OnNMCustomdrawSliderQuickTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_1X, &CMainTab::OnNMCustomdrawSlider1x)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_2X, &CMainTab::OnNMCustomdrawSlider2x)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CANCEL, &CMainTab::OnNMCustomdrawSliderCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIMETODWELL, &CMainTab::OnNMCustomdrawSliderTimetodwell)
END_MESSAGE_MAP()




BOOL CMainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mSliderSpeedHorisontal.SetRange(3, 60);
	mSliderSpeedVertical.SetRange(3, 80);
	mSliderSmileAngle.SetRange(60, 170);
	mSliderSmileSensitivity.SetRange(2, 300);
	mSliderDwellDispl.SetRange(1, 50);
	mSliderDwellTime.SetRange(2, 50);
	mSliderQuickTime.SetRange(2, 40);
	mSlider1x.SetRange(2, 30);
	mSlider2x.SetRange(2, 30);
	mSliderCancel.SetRange(2, 30);
	mSliderTimeToDwell.SetRange(2, 100);

	mSliderSpeedHorisontal.SetPos(iSliderSpeedHorisontal * 10);
	mSliderSpeedVertical.SetPos(iSliderSpeedVertical * 10);
	mSliderSmileAngle.SetPos(iSliderSmileAngle);
	mSliderSmileSensitivity.SetPos(iSliderSmileSensitivity);
	mSliderDwellDispl.SetPos(iSliderDwellDispl * 10);
	mSliderDwellTime.SetPos(iSliderDwellTime * 10);
	mSliderQuickTime.SetPos(iSliderQuickTime * 10);
	mSlider1x.SetPos(iSlider1x * 10);
	mSlider2x.SetPos(iSlider2x * 10);
	mSliderCancel.SetPos(iSliderCancel * 10);
	mSliderTimeToDwell.SetPos(iSliderTimeToDwell * 10);

	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// CMainTab message handlers


void CMainTab::OnNMCustomdrawSliderSpeedHorizontal(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderSpeedHorisontal = (float) mSliderSpeedHorisontal.GetPos() / 10;
	if (iSliderSpeedHorisontal < 0.3) iSliderSpeedHorisontal = 0.3;

	str2.Format(L"%.*f", 1, iSliderSpeedHorisontal);
	str1 = "Horizontal mouse speed (X): ";
	mStaticSpeedHorizontal.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_SPEED_HORIZONTAL, 0);

	*pResult = 0;

}


void CMainTab::OnNMCustomdrawSliderSpeedVertical(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderSpeedVertical = (float)mSliderSpeedVertical.GetPos() / 10;
	if (iSliderSpeedVertical < 0.3) iSliderSpeedVertical = 0.3;

	str2.Format(L"%.*f", 1, iSliderSpeedVertical);
	str1 = "Vertical mouse speed (Y): ";
	mStaticSpeedVertical.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_SPEED_VERTICAL, 0);

	*pResult = 0;

}


void CMainTab::OnNMCustomdrawSliderSmileAngle(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderSmileAngle = (int)mSliderSmileAngle.GetPos() ;
	if (iSliderSmileAngle < 60) iSliderSmileAngle = 60;

	str2.Format(L"%.*i", 0, iSliderSmileAngle);
	str1 = "Smile angle (geometric): ";
	mStaticSmileAngle.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_SMILE_ANGLE, 0);

	*pResult = 0;
}


void CMainTab::OnNMCustomdrawSliderSmileSensitivity(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderSmileSensitivity = (int)mSliderSmileSensitivity.GetPos();
	if (iSliderSmileSensitivity < 2) iSliderSmileSensitivity = 2;

	str2.Format(L"%.*i", 0, iSliderSmileSensitivity);
	str1 = "Recognition threshold (AI): ";
	mStaticSmileSensitivity.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_SMILE_SENSITIVITY, 0);

	*pResult = 0;
}


void CMainTab::OnNMCustomdrawSliderDwellDispl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderDwellDispl = (float)mSliderDwellDispl.GetPos()/10;
	if (iSliderDwellDispl < 0.1) iSliderDwellDispl = 0.1;

	str2.Format(L"%.*f", 1, iSliderDwellDispl);
	str1 = "Dwell displacement: ";
	mStaticDwellDisp.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_DWELL_DISPL, 0);
	*pResult = 0;
}

void CMainTab::OnNMCustomdrawSliderQuickTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderQuickTime = (float)mSliderQuickTime.GetPos()/10;
	if (iSliderQuickTime < 0.2) iSliderQuickTime = 0.2;

	str2.Format(L"%.*f", 1, iSliderQuickTime);
	str1 = "'Quick click' time, (sec): ";
	mStaticQuickTime.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_QUICK_TIME, 0);
	*pResult = 0;

}

void CMainTab::OnNMCustomdrawSliderDwellTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderDwellTime = (float)mSliderDwellTime.GetPos() / 10;
	if (iSliderDwellTime < 0.2) iSliderDwellTime = 0.2;

	str2.Format(L"%.*f", 1, iSliderDwellTime);
	str1 = "Dwell duration, (sec): ";
	mStaticDwellTime.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_DWELL_TIME, 0);
	*pResult = 0;
}


void CMainTab::OnNMCustomdrawSliderTimetodwell(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderTimeToDwell = (float)mSliderTimeToDwell.GetPos() / 10;
	if (iSliderTimeToDwell < 0.2) iSliderTimeToDwell = 0.2;

	str2.Format(L"%.*f", 1, iSliderTimeToDwell);
	str1 = "Time to dwell, (sec): ";
	mStaticTimeToDwell.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_TIMETODWELL, 0);
	*pResult = 0;
}



void CMainTab::OnNMCustomdrawSlider1x(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSlider1x = (float)mSlider1x.GetPos() / 10;
	if (iSlider1x < 0.2) iSlider1x = 0.2;

	str2.Format(L"%.*f", 1, iSlider1x);
	str1 = "Waiting: ";
	mStatic1x.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_1X, 0);
	*pResult = 0;
}


void CMainTab::OnNMCustomdrawSlider2x(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSlider2x = (float)mSlider2x.GetPos() / 10;
	if (iSlider2x < 0.2) iSlider2x = 0.2;

	str2.Format(L"%.*f", 1, iSlider2x);
	str1 = "1x click: ";
	mStatic2x.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_2X, 0);
	*pResult = 0;
}


void CMainTab::OnNMCustomdrawSliderCancel(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderCancel = (float)mSliderCancel.GetPos() / 10;
	if (iSliderCancel < 0.2) iSliderCancel = 0.2;

	str2.Format(L"%.*f", 1, iSliderCancel);
	str1 = "2x click: ";
	mStaticCancel.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_CANCEL, 0);
	*pResult = 0;
}





void CMainTab::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CMainTab::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnCancel();
}
